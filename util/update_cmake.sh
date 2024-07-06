#!/bin/sh

set -e -u

update_cmake.sh \
	libstryker/files.txt \
	libstryker/include \
	libstryker/src
