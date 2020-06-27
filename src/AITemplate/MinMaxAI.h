#pragma once

#include <UltraOOXX/Wrapper/AI.h>
#include <UltraOOXX/UltraBoard.h>
#include <algorithm>
#include <iostream>
#include <random>
#include <ctime>

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
public:
    void init(bool order) override
    {
        // any way
        this->prev_x = -1;
        this->prev_y = -1;
        this->firstOrder = order;
        this->firstOrderCounter = 0;
        this->SameStep = std::pair<int, int>(-1, -1);
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

        // get subboard from prev_pos
        TA::Board sub_board;
        sub_board = main_board.sub(this->prev_x % 3, this->prev_y % 3);
        std::cout << "prev: (" << this->prev_x << ", " << this->prev_y << ")\n";
        std::cout << "sub_board we're going to: (" << this->prev_x %3  << ", " << this->prev_y % 3 << "\n";
        if (firstOrder)
        {
            if (this->prev_x == -1 && this->prev_y == -1)
            {
                step = std::pair<int, int>(4, 4);
                std::cout << "Middle Point\n";
                return step;
            }
            else if (!(sub_board.full()))
            {
                std::cout << "sub board is not full\n";
                // restrict in mid subboard
                if (this->firstOrderCounter != 7)
                {
                    std::cout << "haven't filled mid sub_board\n";
                    PrintState(sub_board.state(1, 1));
                    if (sub_board.state(1, 1) == TA::BoardInterface::Tag::None)
                    {
                        std::cout << "take step at (1,1) of sub_board\n";
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
                    std::cout << "Start to make SameStep\n";
                    if (SameStep == std::pair<int, int>(-1, -1))
                    {
                        step = std::pair<int, int>((prev_x % 3), (prev_y % 3));
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
                            step = std::pair<int, int>(3 - SameStep.first, 3 - SameStep.second);
                        }
                    }
                }
            }
            else
            {
                // 只可能是第一個sameboard時引導至中間的subboard的情況
                sub_board = main_board.sub(3 - prev_x / 3, 3 - prev_y / 3);
                if (sub_board.state(SameStep.first, SameStep.second) == TA::BoardInterface::Tag::None)
                {
                    step = SameStep;
                }
                else
                {
                    step = std::pair<int, int>(3 - SameStep.first, 3 - SameStep.second);
                }
                step.first += (3 - prev_x % 3) * 3;
                step.second += (3 - prev_y % 3) * 3;
                return step;
            }
            step.first += (prev_x / 3) * 3;
            step.second += (prev_y / 3) * 3;
        }
        else // 非first Order
        {
            if (!(sub_board.full()))
            {
                step = findStep(sub_board);
                // 將3X3座標換算成 9x9座標
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
        }
        std::cout << "Going to put at: " << step.first<< ", " << step.second << "\n";
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
    bool firstOrder;
    int firstOrderCounter;
    std::pair<int, int> SameStep; // In the second phase, to make enemy stuck in the same block;
};
