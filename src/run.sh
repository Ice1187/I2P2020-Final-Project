#!/bin/sh

if [[ "$#" -ne 2 ]]; then
    echo 'Usage: ./run.sh <AI 1 type> <AI 2 type>
    
    AI type:
      0: AITemplate
      1: DummyAI
      2: RandomAI
      3: MinMaxAI
      4: FirstWinAI
      5: ManualAI (need to change MAX_TIME in Game.h)
      6: First-Win & Second MinMaxAI
    
    Debug method:
      $ ./run 2 1 > out
      $ less -r out
      (keep pressing f to see the output of every round)
    '>&2
else
    make AITYPE="AITYPE=$1" AI1
    make AITYPE="AITYPE=$2" AI2
    make gamerunner
fi

cd ./build/
COLUMNS=150 ./gamerunner
cd ..


