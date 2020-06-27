#!/bin/sh

if [[ "$#" -ne 2 ]]; then
    echo 'Usage: ./run.sh <AI 1 type> <AI 2 type>' >&2
    echo ''
    echo 'Example: ./run.sh 2 1' >&2
    echo ''
    echo 'AI type:'
    echo '    0: AITemplate' >&2
    echo '    1: DummyAI' >&2
    echo '    2: RandomAI' >&2
    echo '    3: MinMaxAI' >&2
    exit -1
fi

make AITYPE="AITYPE=$1" AI1
make AITYPE="AITYPE=$2" AI2
make gamerunner

cd ./build/
COLUMNS=150 ./gamerunner
cd ..

