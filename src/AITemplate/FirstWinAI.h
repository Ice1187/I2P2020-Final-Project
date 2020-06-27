#pragma once

#include <UltraOOXX/Wrapper/AI.h>
#include <UltraOOXX/UltraBoard.h>
#include <useful/boardUtils.h>


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
    int board_x, board_y;
    bool firstOrder;
    int firstOrderCounter;
    std::pair<int, int> SameStep; // In the second phase, to make enemy stuck in the same block;
public:
    void init(bool order) override
    {
        // any way
        // std::cout << "Order: " << order << "\n";
        // if(order)
        //     std::cout << "We are doing first Order!!\n";
        this->prev_x = this->prev_y = -1;
        this->board_x = this->board_y = 0;

        this->firstOrder = order;
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
        this->board_x = x % 3;
        this->board_y = y % 3;
    }

    std::pair<int, int> queryWhereToPut(TA::UltraBoard main_board) override
    {
        std::pair<int, int> step(-1, -1);
        TA::Board sub_board;

        sub_board = main_board.sub(this->board_x, this->board_y);
        if(this->firstOrder){
            step = this->FirstOrderStep(sub_board, main_board);
        }
        return step;
    }
    std::pair<int, int> FirstOrderStep(TA::Board sub_board, TA::UltraBoard main_board){
        std::pair<int, int> step(-1,-1);
        // std::cout << "prev: (" << this->prev_x << ", " << this->prev_y << ")\n";
        // std::cout << "sub_board we're going to: (" << this->board_x  << ", " << this->board_y << "\n";
        if (this->prev_x == -1 && this->prev_y == -1)
        {
            step = std::pair<int, int>(4, 4);
        }
        else if (!(sub_board.full()))
        {
            // std::cout << "sub board is not full\n";
            // restrict in mid subboard
            if (this->firstOrderCounter != 7)
            {
                // std::cout << "haven't filled mid sub_board\n";
                PrintState(sub_board.state(1, 1));
                if (sub_board.state(1, 1) == TA::BoardInterface::Tag::None)
                {
                    // std::cout << "take step at (1,1) of sub_board\n";
                    this->firstOrderCounter++;
                    step = std::pair<int, int>(1, 1);
                }
                else {
                    std::cout << "[err] sub_board[1, 1] tag is not Tag::None\n";
                    throw "[err] sub_board[1, 1] tag is not Tag::None\n";
                }
            }
            else
            {
                // std::cout << "Start to make SameStep\n";
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
            step.first += board_x * 3;
            step.second += board_y * 3;
        }
        else
        {
            // 只可能是第一個sameboard時引導至中間的subboard的情況
            // std::cout << "Full Case\n";
            std::pair<int, int> subBoardIndex;
            subBoardIndex.first = 2 - SameStep.first;
            subBoardIndex.second = 2 - SameStep.second;
            // std::cout << "Next subBoardIindex (" << subBoardIndex.first << ", " << subBoardIndex.second << ")\n";
            sub_board = main_board.sub(subBoardIndex.first, subBoardIndex.second);
            if (sub_board.state(SameStep.first, SameStep.second) == TA::BoardInterface::Tag::None)
            {
                step = SameStep;
            }
            else
            {
                step = std::pair<int, int>(2 - SameStep.first, 2 - SameStep.second);
            }
            step.first += subBoardIndex.first * 3;
            step.second += subBoardIndex.second *3;
        }
        // std::cout << "Using first Order and now we are going to put at (" << step.first << ", " << step.second << ")\n";
        return step;
    }
    // can only find the best step of given sub board
};
