#!/bin/bash

e-reset

(cd code; make)

ESDK=${EPIPHANY_HOME}
ELIBS=${ESDK}/tools/host/lib:${LD_LIBRARY_PATH}
EHDF=${EPIPHANY_HDF}
ELDF=${ESDK}/bsps/current/internal.ldf

SCRIPT=$(readlink -f "$0")
EXEPATH=$(dirname "$SCRIPT")

e-reset

sudo -E LD_LIBRARY_PATH=${ELIBS} EPIPHANY_HDF=${EHDF} $EXEPATH/fl-host.elf

if [ $? -ne 0 ]
then
    echo "$SCRIPT FAILED"
else
    echo "$SCRIPT PASSED"
fi

e-reset