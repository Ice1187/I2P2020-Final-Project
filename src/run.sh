#!/bin/sh

if [[ "$#" -ne 2 ]]; then
    echo 'Usage: ./run.sh <AI 1 type> <AI 2 type>' >&2
    exit -1
fi

make AITYPE="AITYPE=$1" AI1
make AITYPE="AITYPE=$2" AI2
make gamerunner

cd ./build/
./gamerunner
cd ..

