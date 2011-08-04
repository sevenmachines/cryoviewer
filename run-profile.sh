#!/bin/sh
LD_LIBRARY_PATH=../cryomesh/Debug/:../cryomesh-common/Debug/:../cryoviewer-common/Debug/  valgrind --tool=callgrind ./Debug/cryoviewer $@
