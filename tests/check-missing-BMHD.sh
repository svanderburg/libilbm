#!/bin/sh

./checkilbm missing-BMHD.ILBM

if test $? = 0
then
    exit 1
else
    exit 0
fi
