#pragma once

#include <UltraOOXX/Wrapper/AI.h>
#include <UltraOOXX/UltraBoard.h>
#include <algorithm>
#include <random>
#include <ctime>

class AI : public AIInterface
{
public:
    void init(bool order) override
    {
        // any way
    }

    void callbackReportEnemy(int x, int y) override
    {
        (void) x;
        (void) y;
        // give last step
    }

    std::pair<int,int> queryWhereToPut(TA::UltraBoard) override
    {
        return std::make_pair(1,2);
    }
};
