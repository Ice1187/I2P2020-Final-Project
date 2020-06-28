#pragma once

#include <UltraOOXX/Wrapper/AI.h>
#include <UltraOOXX/UltraBoard.h>
#include <useful/BoardUtils.h>
#include <algorithm>
#include <cstdio>
class ManualAI : public AIInterface
{
public:
    void init(bool order) override
    {
        // any way
        this->order = order;
        this->prev_x = -1;
        this->prev_y = -1;
        this->first = order;
    }

    void callbackReportEnemy(int x, int y) override
    {
        // LFs Wang said following 2 line is the hint to the bonus
        (void)x;
        (void)y;

        // give last step
        this->prev_x = x;
        this->prev_y = y;
    }

    std::pair<int, int> queryWhereToPut(TA::UltraBoard mainBoard) override
    {
        std::pair<int, int> step(-1, -1);

        // if (first)
        // {
        //     first = false;
        //     printf("target range: [0-8, 0-8]\n");
        //     scanf("%d %d", &(step.first), &(step.second));
        //     printf("%d, %d\n", step.first, step.second);
        //     while (step.first < 0 || step.first >= 9 || step.second < 0 || step.second >= 9)
        //     {
        //         printf("target range: [0-8, 0-8]\n");
        //         scanf("%d %d", &(step.first), &(step.second));
        //     }
        //     return step;
        // }

        while (!isValid(step, mainBoard))
            scanf("%d %d", &(step.first), &(step.second));

        return step;
    }

    bool isValid(std::pair<int, int> &pos, TA::UltraBoard mainBoard)
    {

        auto [targetBoard_x, targetBoard_y] = US::getNextSubboardId(prev_x, prev_y);
        TA::Board targetBoard = mainBoard.sub(targetBoard_x, targetBoard_y);

        if (!targetBoard.full() && targetBoard_x != -1)
        {
            printf("target range: [%d-%d, %d-%d]\n", targetBoard_x * 3, targetBoard_x * 3 + 2, targetBoard_y * 3, targetBoard_y * 3 + 2);
            if (pos.first == -1)
                return false;
            else if ((!US::inSubboard(pos.first, pos.second, targetBoard_x, targetBoard_y)))
            {
                printf("[!] (%d, %d) not in target range\n", pos.first, pos.second);
                return false;
            }
            else if (mainBoard.get(pos.first, pos.second) != TA::BoardInterface::Tag::None)
            {
                printf("[!] (%d, %d) not stepable\n", pos.first, pos.second);
                return false;
            }
        }
        else
        {
            printf("target range: [0-8, 0-8]\n");
            if (pos.first == -1)
                return false;
            if (mainBoard.get(pos.first, pos.second) != TA::BoardInterface::Tag::None)
            {
                printf("[!] (%d, %d) not stepable\n", pos.first, pos.second);
                return false;
            }
        }

        return true;
    }

private:
    int prev_x,
        prev_y;
    int order;
    bool first;
};
