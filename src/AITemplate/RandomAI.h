#pragma once

#include <UltraOOXX/Wrapper/AI.h>
#include <UltraOOXX/UltraBoard.h>
#include <useful/BoardUtils.h>
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>

class RandomAI : public AIInterface
{
public:
    void init(bool order) override
    {
        // any way
        this->order = order;
        this->board_x = 0;
        this->board_y = 0;
        srand(clock());
    }

    void callbackReportEnemy(int x, int y) override
    {
        // LFs Wang said following 2 line is the hint to the bonus
        (void)x;
        (void)y;

        // give last step
        this->board_x = x % 3;
        this->board_y = y % 3;
    }

    std::pair<int, int> queryWhereToPut(TA::UltraBoard main_board) override
    {
        std::pair<int, int> step(-1, -1);
        auto [targetBoard_x, targetBoard_y] = US::getNextSubboardId(board_x, board_y);
        TA::Board target_board;

        target_board = main_board.sub(targetBoard_x, targetBoard_y);

        if (!(target_board.full()))
        {
            step = findStep(target_board);
            step.first += targetBoard_x * 3;
            step.second += targetBoard_y * 3;
        }
        else
        {
            int selectBoard_x = rand() % 3;
            int selectBoard_y = rand() % 3;
            TA::Board selectBoard = main_board.sub(selectBoard_x, selectBoard_y);
            while (selectBoard.full())
            {
                selectBoard_x = rand() % 3;
                selectBoard_y = rand() % 3;
                selectBoard = main_board.sub(selectBoard_x, selectBoard_y);
            }

            step = findStep(selectBoard);
            step.first += selectBoard_x * 3;
            step.second += selectBoard_y * 3;
        }

        return step;
    }

    std::pair<int, int> findStep(const TA::Board board)
    {
        int x = rand() % 3, y = rand() % 3;
        while (board.state(x, y) != TA::BoardInterface::Tag::None)
        {
            x = rand() % 3;
            y = rand() % 3;
        }

        return std::make_pair(x, y);
    }

private:
    int board_x, board_y;
    int order;
};
