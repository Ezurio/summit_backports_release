#!/usr/bin/env bash

set -e -u -o pipefail

ROOT_DIR=${1:-/devel/backports/ksrc-backports}
REPORT_DIR=${2:-build-reports}
KERNEL_FILTER_ARG=${3:-}

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
REPO_ROOT=$(cd -- "$SCRIPT_DIR/.." && pwd)
REPORT_DIR_ABS=$REPORT_DIR
ARTIFACT_DIR=

case $REPORT_DIR_ABS in
/*)
	;;
*)
	REPORT_DIR_ABS="$REPO_ROOT/$REPORT_DIR_ABS"
	;;
esac

case $REPORT_DIR_ABS in
""|/|"$REPO_ROOT")
	echo "refusing to clean report directory: $REPORT_DIR_ABS" >&2
	exit 1
	;;
esac

rm -rf -- "$REPORT_DIR_ABS"
mkdir -p "$REPORT_DIR_ABS/logs" "$REPORT_DIR_ABS/results" "$REPORT_DIR_ABS/artifacts"

REPORT_FILE="$REPORT_DIR_ABS/kernel-build-report.txt"
SUCCESS_FILE="$REPORT_DIR_ABS/kernel-build-successes.txt"
FAIL_FILE="$REPORT_DIR_ABS/kernel-build-failures.txt"
RESULT_DIR="$REPORT_DIR_ABS/results"
ARTIFACT_DIR="$REPORT_DIR_ABS/artifacts"

cpu_count=$(nproc 2>/dev/null || echo 1)
if [ "$cpu_count" -lt 1 ]; then
	cpu_count=1
fi

MATRIX_JOBS=${MATRIX_JOBS:-$cpu_count}
KBUILD_JOBS=${KBUILD_JOBS:-$cpu_count}

if [ -n "$KERNEL_FILTER_ARG" ]; then
	KERNEL_FILTER=$KERNEL_FILTER_ARG
else
	KERNEL_FILTER=${KERNEL_FILTER:-}
fi
USE_CLANG=${USE_CLANG:-0}
CLANG_REQUESTED=0
BUILD_CC=

if [ -n "${CC:-}" ]; then
	BUILD_CC=$CC
	case $BUILD_CC in
	*clang*|*clang-kbuild-x*)
		CLANG_REQUESTED=1
		;;
	esac
elif [ "$USE_CLANG" = 1 ] || [ "$USE_CLANG" = y ] || [ "$USE_CLANG" = yes ] || \
	[ "$USE_CLANG" = true ]; then
	if [ ! -f "$SCRIPT_DIR/clang-kbuild-x" ]; then
		echo "clang wrapper not found: $SCRIPT_DIR/clang-kbuild-x" >&2
		exit 1
	fi
	BUILD_CC="bash $SCRIPT_DIR/clang-kbuild-x"
	CLANG_REQUESTED=1
fi

if [ ! -d "$ROOT_DIR" ]; then
	echo "kernel root not found: $ROOT_DIR" >&2
	exit 1
fi

declare -A KERNELS=()

kernel_matches_filter()
{
	local label=$1
	local match_label=${label#linux-headers-}

	match_label=${match_label#linux-}

	if [ -z "$KERNEL_FILTER" ]; then
		return 0
	fi

	case $match_label in
	"$KERNEL_FILTER" | "$KERNEL_FILTER"[-.]*)
		return 0
		;;
	esac

	return 1
}

add_kernel_root()
{
	local label=$1
	local path=$2
	local real_path

	kernel_matches_filter "$label" || return 0

	[ -d "$path" ] || return 0
	[ -f "$path/Makefile" ] || return 0

	real_path=$(readlink -f -- "$path")
	[ -n "$real_path" ] || return 0

	if [ -z "${KERNELS[$real_path]+x}" ]; then
		KERNELS[$real_path]=$label
	fi
}

while IFS= read -r build_link; do
	module_dir=$(dirname -- "$build_link")
	label=$(basename -- "$module_dir")
	add_kernel_root "$label" "$build_link"
done < <(find "$ROOT_DIR/lib/modules" -mindepth 2 -maxdepth 2 -type l -name build 2>/dev/null | sort)

while IFS= read -r header_dir; do
	label=$(basename -- "$header_dir")
	add_kernel_root "$label" "$header_dir"
done < <(find "$ROOT_DIR/usr/src" -mindepth 1 -maxdepth 1 -type d -name 'linux-headers-*-generic' 2>/dev/null | sort)

while IFS= read -r source_dir; do
	label=$(basename -- "$source_dir")
	add_kernel_root "$label" "$source_dir"
done < <(find "$ROOT_DIR/rhel" "$ROOT_DIR/vendor_kernel" -mindepth 1 -maxdepth 1 -type d 2>/dev/null | sort)

if [ "${#KERNELS[@]}" -eq 0 ]; then
	if [ -n "$KERNEL_FILTER" ]; then
		echo "no kernel build roots found under $ROOT_DIR matching version prefix: $KERNEL_FILTER" >&2
	else
	echo "no kernel build roots found under $ROOT_DIR" >&2
	fi
	exit 1
fi

mapfile -t SORTED_KERNELS < <(
	for path in "${!KERNELS[@]}"; do
		printf '%s\t%s\n' "${KERNELS[$path]}" "$path"
	done | sort -V -k1,1
)

{
	echo "qcacld multi-kernel build report"
	echo "repo: $REPO_ROOT"
	echo "kernel root: $ROOT_DIR"
	if [ -n "$KERNEL_FILTER" ]; then
		echo "kernel filter: $KERNEL_FILTER"
	fi
	echo "report generated: $(date -Iseconds)"
	if [ "$CLANG_REQUESTED" -eq 1 ]; then
		echo "compiler request: clang on supported kernels only"
		echo "clang wrapper: $BUILD_CC"
	elif [ -n "$BUILD_CC" ]; then
		echo "compiler: $BUILD_CC"
	else
		echo "compiler: default"
	fi
	echo "temporary build directories: $ARTIFACT_DIR"
	echo
	printf '%-35s %-6s %s\n' "kernel" "result" "path"
	printf '%-35s %-6s %s\n' "------" "------" "----"
} > "$REPORT_FILE"

: > "$SUCCESS_FILE"
: > "$FAIL_FILE"

pass_count=0
fail_count=0
total_count=0

copy_source_tree()
{
	local destination=$1

	if command -v rsync >/dev/null 2>&1; then
		rsync -a --delete \
			--exclude='.git/' \
			--exclude='build-reports/' \
			--exclude='.tmp_versions/' \
			--exclude='*.o' \
			--exclude='*.ko' \
			--exclude='*.mod' \
			--exclude='*.mod.c' \
			--exclude='*.cmd' \
			--exclude='Module.symvers' \
			--exclude='modules.order' \
			--exclude='wlan.mod' \
			--exclude='wlan.mod.c' \
			--exclude='wlan.mod.o' \
			--exclude='wlan.o' \
			--exclude='wlan.ko' \
			"$REPO_ROOT/" "$destination/"
	else
		cp -a "$REPO_ROOT/." "$destination/"
		rm -rf "$destination/.git" "$destination/build-reports" "$destination/.tmp_versions"
		find "$destination" -type f \
			\( -name '*.o' -o -name '*.ko' -o -name '*.mod' -o -name '*.mod.c' \
			-o -name '*.cmd' -o -name 'Module.symvers' -o -name 'modules.order' \) -delete
	fi
}

kernel_supports_clang()
{
	local kernel_root=$1

	if [ "$CLANG_REQUESTED" -eq 0 ]; then
		return 1
	fi

	if ! command -v clang >/dev/null 2>&1; then
		return 1
	fi

	if rg -q 'CONFIG_CC_IS_CLANG|cc-is-clang|CONFIG_CLANG_VERSION|LLVM_IAS|scripts/Makefile\.clang|CLANG_FLAGS' \
		"$kernel_root" -g 'Makefile' -g '*.mk' -g 'scripts/**' -g 'include/**' 2>/dev/null; then
		return 0
	fi

	return 1
}

run_make()
{
	local kernel_root=$1
	shift
	local build_cc=${1:-}
	if [ "$#" -gt 0 ]; then
		shift
	fi

	if [ -n "$build_cc" ]; then
		make KERNEL_SRC="$kernel_root" CC="$build_cc" OBJECT_FILES_NON_STANDARD=y "$@"
	else
		make KERNEL_SRC="$kernel_root" OBJECT_FILES_NON_STANDARD=y "$@"
	fi
}

build_one()
{
	local label=$1
	local kernel_root=$2
	local safe_label=${label//\//_}
	local log_file="$REPORT_DIR_ABS/logs/${safe_label}.log"
	local result_file="$RESULT_DIR/${safe_label}.status"
	local workspace="$ARTIFACT_DIR/${safe_label}"
	local build_root="$workspace/qcacld"
	local result=FAIL
	local build_mode=in-tree-serial
	local artifact_path="$workspace"
	local build_cc=
	local compiler_mode=default

	if [ "$MATRIX_JOBS" -gt 1 ]; then
		build_mode=workspace-copy
		rm -rf "$workspace"
		mkdir -p "$workspace"
	fi

	if [ -n "$BUILD_CC" ]; then
		if [ "$CLANG_REQUESTED" -eq 0 ] || kernel_supports_clang "$kernel_root"; then
			build_cc=$BUILD_CC
			if [ "$CLANG_REQUESTED" -eq 0 ]; then
				compiler_mode=requested
			else
				compiler_mode=clang
			fi
		elif [ "$CLANG_REQUESTED" -eq 1 ]; then
			compiler_mode=default-clang-unsupported
		fi
	fi

	{
		echo "==> kernel: $label"
		echo "==> path:   $kernel_root"
		echo "==> start:  $(date -Iseconds)"
		echo "==> build mode: $build_mode"
		echo "==> compiler mode: $compiler_mode"
		echo "==> matrix jobs: $MATRIX_JOBS"
		echo "==> kbuild jobs: $KBUILD_JOBS"
		if [ -n "$build_cc" ]; then
			echo "==> compiler: $build_cc"
		fi
		if [ "$build_mode" = workspace-copy ]; then
			echo "==> artifact dir: $artifact_path"
		fi
		echo
	} > "$log_file"

	if [ "$build_mode" = workspace-copy ]; then
		if ! copy_source_tree "$build_root" >> "$log_file" 2>&1; then
			printf 'label=%s\nresult=FAIL\nkernel_root=%s\nlog_file=%s\nartifact_dir=%s\nbuild_mode=%s\n' \
				"$label" "$kernel_root" "$log_file" "$artifact_path" "$build_mode" > "$result_file"
			rm -rf "$workspace"
			return 0
		fi
	else
		build_root="$REPO_ROOT"
	fi

	(
		cd "$build_root" || exit 1
		run_make "$kernel_root" "$build_cc" clean
	) >> "$log_file" 2>&1 || true

	if (
		cd "$build_root" || exit 1
		run_make "$kernel_root" "$build_cc" -k -j"$KBUILD_JOBS"
	) >> "$log_file" 2>&1; then
		result=PASS
	fi

	(
		cd "$build_root" || exit 1
		run_make "$kernel_root" "$build_cc" clean
	) >> "$log_file" 2>&1 || true

	printf 'label=%s\nresult=%s\nkernel_root=%s\nlog_file=%s\nartifact_dir=%s\nbuild_mode=%s\ncompiler_mode=%s\n' \
		"$label" "$result" "$kernel_root" "$log_file" "$artifact_path" "$build_mode" "$compiler_mode" > "$result_file"

	if [ "$build_mode" = workspace-copy ]; then
		rm -rf "$workspace"
	fi
}

active_jobs=0
for entry in "${SORTED_KERNELS[@]}"; do
	label=${entry%%$'\t'*}
	kernel_root=${entry#*$'\t'}
	total_count=$((total_count + 1))

	build_one "$label" "$kernel_root" &
	active_jobs=$((active_jobs + 1))

	if [ "$active_jobs" -ge "$MATRIX_JOBS" ]; then
		wait -n
		active_jobs=$((active_jobs - 1))
	fi
done

while [ "$active_jobs" -gt 0 ]; do
	wait -n
	active_jobs=$((active_jobs - 1))
done

for entry in "${SORTED_KERNELS[@]}"; do
	label=${entry%%$'\t'*}
	kernel_root=${entry#*$'\t'}
	safe_label=${label//\//_}
	result_file="$RESULT_DIR/${safe_label}.status"
	log_file="$REPORT_DIR_ABS/logs/${safe_label}.log"
	result=FAIL

	if [ -f "$result_file" ]; then
		# shellcheck disable=SC1090
		source "$result_file"
	fi

	printf '%-35s %-6s %s\n' "$label" "$result" "$kernel_root" >> "$REPORT_FILE"

	if [ "$result" = PASS ]; then
		pass_count=$((pass_count + 1))
		printf '%s\t%s\n' "$label" "$kernel_root" >> "$SUCCESS_FILE"
	else
		fail_count=$((fail_count + 1))
		printf '%s\t%s\n' "$label" "$kernel_root" >> "$FAIL_FILE"
		{
			echo
			echo "[$label] first error lines:"
			if ! grep -E -m 20 '(^|[^[:alpha:]])(error:|Error |No rule to make target|No such file or directory|No targets specified)' "$log_file"; then
				tail -n 20 "$log_file"
			fi
			echo
		} >> "$REPORT_FILE"
	fi
done

{
	echo
	echo "summary"
	echo "-------"
	echo "total kernels: $total_count"
	echo "passed: $pass_count"
	echo "failed: $fail_count"
	echo
	echo "temporary build directories: $ARTIFACT_DIR"
	echo "full logs: $REPORT_DIR_ABS/logs"
	echo "success list: $SUCCESS_FILE"
	echo "failure list: $FAIL_FILE"
} >> "$REPORT_FILE"

cat "$REPORT_FILE"

if [ "$fail_count" -ne 0 ]; then
	exit 1
fi
