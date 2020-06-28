#!/bin/sh

if [[ "$#" -ne 2 ]]; then
    echo 'Usage: ./run.sh <AI 1 type> <AI 2 type>' >&2
    echo ''
    echo 'AI type:'
    echo '  0: AITemplate' >&2
    echo '  1: DummyAI' >&2
    echo '  2: RandomAI' >&2
    echo '  3: MinMaxAI' >&2
    echo '  4: FirstWinAI' >&2
    echo '  5: ManualAI (need to change MAX_TIME in Game.h)' >&2
    echo ''
    echo 'Debug method:'
    echo '  $ ./run 2 1 > out'>&2
    echo '  $ less -r out'
    echo '  (keep pressing f to see the output of every round)'

    AI type:
      0: AITemplate
      1: DummyAI
      2: RandomAI
      3: MinMaxAI
      4: FirstWinAI
      5: ManualAI (need to change MAX_TIME in Game.h)
    
    Debug method:
      $ ./run 2 1 > out
      $ less -r out
      (keep pressing f to see the output of every round)
      '
else
    make AITYPE="AITYPE=$1" AI1
    make AITYPE="AITYPE=$2" AI2
    make gamerunner
fi

cd ./build/
COLUMNS=150 ./gamerunner
cd ..


