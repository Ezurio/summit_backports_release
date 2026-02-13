#!/bin/sh

set -e

. ./scripts/mod_helpers.sh

case "$(mod_filename compat)" in
	*.gz)
		compr=".gz"
		;;
	*.xz)
		compr=".xz"
		;;
	*.zst)
		compr=".zst"
		;;
	*)
		compr=
		;;
esac

find "${BACKPORT_DIR}" -type f -name '*.ko' |\
while read -r driver; do
	mod_name=${KLIB}${KMODDIR}${driver#"${BACKPORT_DIR}"}${compr}
	echo "  uninstall ${mod_name}"
	rm -f "${mod_name}"
done
