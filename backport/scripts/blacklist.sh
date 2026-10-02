#!/bin/sh

BLACKLIST_CONF="/etc/modprobe.d/backports.conf"
BLACKLIST_MAP=".blacklist.map"

MODULE_DIR=$1
MODULE_UPDATES=$2

[ -d "${MODULE_DIR}" ] || exit
[ -d "${MODULE_UPDATES}" ] || exit

mkdir -p "$(dirname "${BLACKLIST_CONF}")"
rm -f "${BLACKLIST_CONF}"

echo "# To be used when using backported drivers" > "${BLACKLIST_CONF}"

grep -v '^#' "${BLACKLIST_MAP}" |\
while read -r MODULE_UPDATE MODULE_NAME _ ; do
	MODULE="${MODULE_NAME}.ko"

	COUNT=$(find "${MODULE_DIR}" -type f -name "${MODULE}" -or -name "${MODULE}.gz" | wc -l)
	COUNT_REPLACE=$(find "${MODULE_UPDATES}" -type f -name "${MODULE_UPDATE}" -or -name "${MODULE_UPDATE}.gz" | wc -l)

	if [ "${COUNT}" -ne 0 ] && [ "${COUNT_REPLACE}" -ne 0 ]; then
		echo "Blacklisting ${MODULE} ..."
		echo "blacklist ${MODULE_NAME}" >> "${BLACKLIST_CONF}"
	fi
done
