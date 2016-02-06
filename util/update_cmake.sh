#!/bin/sh

set -e -u

update_cmake \
	libstryker/CMakeLists.txt \
	LIBSTRYKER_FILES \
	libstryker/include \
	libstryker/src
