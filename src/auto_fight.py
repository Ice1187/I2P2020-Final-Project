#!/usr/bin/python3

import subprocess
import os
import time

print("""
This script run `./run.sh`, so make sure you have compiled the program and had correct `a1.so`, `a2.so`, and `gamerunner` in `./build`.
""")

command = "./auto_fight.sh"
result = []
rounds = int(input('[+] enter rounds: '))


o_win = 0
x_win = 0
tie = 0
unknown = 0
fail = 0

start_time = time.time()
end_time = time.time()

for i in range(1, rounds+1):
    result.append(subprocess.run(
        [command], stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=2, shell=True))
    if(i % 100 == 0):
        print("[-] Round {} ({:.2f}s)".format(i, time.time() - end_time))
        end_time = time.time()
        
end_time = time.time()

for r in result:
    if r.returncode == 0:
        out = r.stdout[-100:]
        if b"O win" in out:
            o_win += 1
        elif b"X win" in out:
            x_win += 1
        elif b"Tie" in out:
            tie += 1
        else:
            unknown += 1
    else:
        fail = 0

winer = max(o_win, x_win, tie)


def color(x): return '\033[1m\033[31m' + str(x) + \
    '\033[0m' if x == winer else str(x)


print("""
------------------------
Rounds: {}
Times:  {:.2f}s

O win:  {} ({:.1%})
X win:  {} ({:.1%})
Tie:    {} ({:.1%})
Fail:   {} ({:.1%})
Unknow: {} ({:.1%})
""".format(rounds, end_time-start_time,  color(o_win), o_win/rounds, color(x_win), x_win/rounds, color(tie), tie/rounds, fail, fail/rounds, unknown, unknown/rounds))
