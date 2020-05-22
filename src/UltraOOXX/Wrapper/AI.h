#pragma once

#include <UltraOOXX/UltraBoard.h>

#include <chrono>
#include <vector>
#include <utility>

#define AI_ABI_VER 20200516

class AIInterface
{
public:
    AIInterface() = default;
    virtual ~AIInterface() = default;

    int abi(){ return AI_ABI_VER; }

    virtual void init(bool order) = 0;

    virtual void callbackReportEnemy(int x, int y) = 0;

    virtual std::pair<int,int> queryWhereToPut(TA::UltraBoard) = 0;
};
