#pragma once

#include <UltraOOXX/Wrapper/AI.h>
#include <UltraOOXX/UltraBoard.h>
#include <useful/BoardUtils.h>

// #include "../UltraOOXX/Wrapper/AI.h"
// #include "../UltraOOXX/UltraBoard.h"
// #include "../useful/boardUtils.h>

#include <algorithm>
#include <random>
#include <ctime>

#define INF 2147483647
void PrintState(TA::BoardInterface::Tag tag)
{
    if (tag == TA::BoardInterface::Tag::None)
    {
        std::cout << "This tag is None\n";
    }
    else if (tag == TA::BoardInterface::Tag::O)
    {
        std::cout << "This tag is O\n";
    }
    else if (tag == TA::BoardInterface::Tag::X)
    {
        std::cout << "This tag is X\n";
    }
    else if (tag == TA::BoardInterface::Tag::Tie)
    {
        std::cout << "This tag is Tie\n";
    }
    else
        std::cout << "What is this tag???\n";
}
class FirstWinAI : public AIInterface
{
private:
    int prev_x, prev_y;
    int subboard_x, subboard_y;
    bool Order;
    int firstOrderCounter;
    TA::UltraBoard all_board;
    std::pair<int, int> SameStep; // In the second phase, to make enemy stuck in the same block;
public:
    void init(bool order) override
    {
        // any way
        // std::cout << "Order: " << order << "\n";
        // if(order)
        //     std::cout << "We are doing first Order!!\n";
        this->prev_x = this->prev_y = -1;
        this->subboard_x = this->subboard_y = 0;

        this->Order = order;
        this->firstOrderCounter = 0;
        this->SameStep = std::pair<int, int>(-1, -1);
    }

    void callbackReportEnemy(int x, int y) override
    {
        (void)x;
        (void)y;
        // give last step
        this->prev_x = x;
        this->prev_y = y;
        this->subboard_x = x % 3;
        this->subboard_y = y % 3;
    }

    std::pair<int, int> queryWhereToPut(TA::UltraBoard main_board) override
    {
        this->all_board = main_board;
        std::pair<int, int> step(-1, -1);
        TA::Board sub_board;

        sub_board = main_board.sub(this->subboard_x, this->subboard_y);
        if (this->Order)
        {
            step = this->FirstOrderStep(sub_board);
        }
        return step;
    }
    std::pair<int, int> FirstOrderStep(TA::Board sub_board)
    {
        std::pair<int, int> step(-1, -1);
        if (this->prev_x == -1 && this->prev_y == -1)
        {
            step = std::pair<int, int>(4, 4);
        }
        else if (!(sub_board.full()))
        {
            // restrict in mid subboard
            if (this->firstOrderCounter != 7)
            {
                PrintState(sub_board.state(1, 1));
                if (sub_board.state(1, 1) == TA::BoardInterface::Tag::None)
                {
                    this->firstOrderCounter++;
                    step = std::pair<int, int>(1, 1);
                }
                // else
                // {
                //     std::cout << "[err] sub_board[1, 1] tag is not Tag::None\n";
                //     throw "[err] sub_board[1, 1] tag is not Tag::None\n";
                // }
            }
            else
            {
                if (SameStep == std::pair<int, int>(-1, -1))
                {
                    step = US::getNextSubboardId(prev_x, prev_y);
                    SameStep = step;
                }
                else
                {
                    if (sub_board.state(SameStep.first, SameStep.second) == TA::BoardInterface::Tag::None)
                    {
                        step = SameStep;
                    }
                    else
                    {
                        step = std::pair<int, int>(2 - SameStep.first, 2 - SameStep.second);
                    }
                }
            }
            step.first += subboard_x * 3;
            step.second += subboard_y * 3;
        }
        else
        {
            // 只可能是第一個sameboard時引導至中間的subboard的情況
            std::pair<int, int> subBoardIndex; // real going to suboard
            subBoardIndex.first = 2 - SameStep.first;
            subBoardIndex.second = 2 - SameStep.second;
            sub_board = this->all_board.sub(subBoardIndex.first, subBoardIndex.second);
            if (sub_board.state(SameStep.first, SameStep.second) == TA::BoardInterface::Tag::None)
            {
                step = SameStep;
            }
            else
            {
                step = std::pair<int, int>(2 - SameStep.first, 2 - SameStep.second);
            }
            step.first += subBoardIndex.first * 3;
            step.second += subBoardIndex.second * 3;
        }
        return step;
    }
    // can only find the best step of given sub board
};
