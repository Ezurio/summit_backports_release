#!/bin/sh

set -e

. ./scripts/mod_helpers.sh

compr=$(mod_filename mac80211)

find "${1}" -type f -name '*.ko' |\
while read -r driver; do
	case "${compr}" in
		*.gz)
			echo "COMPRESS ${driver}"
			gzip -9 "${driver}"
			;;
		*.xz)
			echo "COMPRESS ${driver}"
			xz -9 "${driver}"
			;;
		*.zst)
			echo "COMPRESS ${driver}"
			zstd -9 "${driver}"
			;;
	esac
done
