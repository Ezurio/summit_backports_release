#!/bin/sh

mod_filename()
{
	if command -v modinfo > /dev/null 2>&1; then
		MOD_QUERY="modinfo -F filename"
	else
		MOD_QUERY="modprobe -l"
	fi
	mod_path="$(${MOD_QUERY} "${1}" | tail -1)"
	basename "${mod_path}"
}
