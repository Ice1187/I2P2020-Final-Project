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
        this->order = order;
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
        std::pair<int, int> target_board_id(prev_x % 3, prev_y % 3);
        TA::Board target_board;

        target_board = main_board.sub(target_board_id.first, target_board_id.second);

        if (!(target_board.full()))
        {
            step = findStep(target_board);
            step.first += target_board_id.first * 3;
            step.second += target_board_id.second * 3;
        }
        else
        {
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    target_board = main_board.sub(i, j);
                    if (!(target_board.full()))
                    {
                        step = findStep(target_board);
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
    int order;
};
