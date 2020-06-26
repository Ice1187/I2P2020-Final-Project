#pragma once

#include <UltraOOXX/Wrapper/AI.h>
#include <UltraOOXX/UltraBoard.h>
#include <algorithm>
#include <random>
#include <ctime>

class DummyAI : public AIInterface
{
public:
    void init(bool order) override
    {
        // any way
        this->prev_x = 0;
        this->prev_y = 0;
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

    std::pair<int, int> queryWhereToPut(TA::UltraBoard main_board) override
    {
        std::pair<int, int> step(-1, -1);
        TA::Board sub_board;

        sub_board = main_board.sub(prev_x / 3, prev_y / 3);
        if (!(sub_board.full()))
        {
            step = findStep(sub_board);
            step.first += (prev_x / 3) * 3;
            step.second += (prev_y / 3) * 3;
        }
        else
        {
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    sub_board = main_board.sub(i, j);
                    if (!(sub_board.full()))
                    {
                        step = findStep(sub_board);
                        step.first += i * 3;
                        step.second += j * 3;
                        break;
                    }
                }
                if (step.first != -1)
                    break;
            }
        }

        return step;
    }

    std::pair<int, int> findStep(const TA::Board board)
    {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (board.state(i, j) == TA::BoardInterface::Tag::None)
                    return std::make_pair(i, j);
        return std::make_pair(-1, -1);
    }

private:
    int prev_x, prev_y;
};
