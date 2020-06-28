# I2P2020 Final Project

The final project of NTHU CS135601.

Team: Dorm Li-cursion

## How to Contribute

Please see the [CONTRIBUTING.md](./CONTRIBUTING.md).

## AI

There are 6 AIs in `src/AITemplate/`. Every AI has it's own strategy to take steps, describing as follow:

- DummyAI: Always search stepable place in same order, just a POC of AI.
- RandomAI: Step according to random, using srand(clock()) as random seed.
- FirstWinAI: Can only play when it's first-move, but always win while first-move.
- ManualAI: Human Player Mode. Needs the player to enter every step.
- MinMaxAI: Using MiniMax Algorithm + Alpha-beta Pruning as it's step strategy. Our best AI in second-move.
- FwSmmAI: First-Win Second-MiniMax AI. Combine FirstWinAI and MinMaxAI, our final AI.

## Compile

- `./run.sh`: `./run.sh <AI 1 type> <AI 2 type>` will compile `a1.so`, `a2.so` as selected AI type, then compile and execute `gamerunner`.
- `make`: Compile `a1.so` and `a2.so` as FwSmmAI, then compile and execute `gamerunner`.

## Tools

- `auto_fight.py`: Let `a1.so` and `a2.so` play n rounds, and output the statistics of the results.
