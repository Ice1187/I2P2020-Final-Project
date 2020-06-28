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
class FwSmmAI : public AIInterface
{
private:
    int prev_x, prev_y;
    int subboard_x, subboard_y;
    bool Order;
    int firstOrderCounter;
    TA::UltraBoard all_board;
    std::pair<int, int> SameStep; // In the second phase, to make enemy stuck in the same block;
    
    TA::BoardInterface::Tag player2tag(bool player)
    {
        if (player == true)
            return TA::BoardInterface::Tag::O;
        else
            return TA::BoardInterface::Tag::X;
    }
public:
    void init(bool order) override
    {
        this->prev_x = this->prev_y = -1;
        this->subboard_x = this->subboard_y = 0;

        this->Order = order;
        this->firstOrderCounter = 0;
        this->SameStep = std::pair<int, int>(-1, -1);
        
        srand(time(nullptr));
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
        std::pair<int, int> next_step;
        TA::Board sub_board;

        sub_board = main_board.sub(this->subboard_x, this->subboard_y);
        if (this->Order)
        {
            next_step = this->FirstOrderStep(sub_board);
        }else{
            next_step = this->MinMaxStep(sub_board);
        }
        return next_step;
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
    std::pair<int, int> MinMaxStep(TA::Board subboard){
        int eval;
        int minEval = INF;
        int maxEval = -INF;
        std::pair<int, int> cur_step, best_step;
        
        if (!subboard.full())
        {
            for (int i = 0; i < 3; i++)
            {
                cur_step.first = i + this->subboard_x * 3;
                for (int j = 0; j < 3; j++)
                {
                    if (subboard.state(i, j) == TA::BoardInterface::Tag::None)
                    {
                        cur_step.second = j + this->subboard_y * 3;
                        eval = minmax(cur_step, 3, -INF, INF, this->Order);
                        if (this->Order == true && eval > maxEval)
                        {
                            best_step = cur_step;
                            maxEval = eval;
                        }
                        else if (this->Order == false && eval < minEval)
                        {
                            best_step = cur_step;
                            minEval = eval;
                        }
                    }
                }
            }
        }
        else
        {
            for (int subboard_index_x = 0; subboard_index_x < 3; subboard_index_x++)
                for (int subboard_index_y = 0; subboard_index_y < 3; subboard_index_y++)
                {
                    subboard = this->all_board.sub(subboard_index_x, subboard_index_y);
                    for (int i = 0; i < 3; i++)
                    {
                        cur_step.first = i + subboard_index_x * 3;
                        for (int j = 0; j < 3; j++)
                        {
                            if (subboard.state(i, j) == TA::BoardInterface::Tag::None)
                            {
                                cur_step.second = j + subboard_index_y * 3;
                                eval = minmax(cur_step, 3, -INF, INF, this->Order);
                                if (this->Order == true && eval > maxEval)
                                {
                                    best_step = cur_step;
                                    maxEval = eval;
                                }
                                else if (this->Order == false && eval < minEval)
                                {
                                    best_step = cur_step;
                                    minEval = eval;
                                }
                            }
                        }
                    }
                }
        }
        return best_step;
    }
    int minmax(std::pair<int, int> &pos, int depth, int alpha, int beta, bool player)
    {
        if (depth == 0)
        {
            if (this->all_board.get(pos.first, pos.second) == TA::BoardInterface::Tag::None)
            {
                this->all_board.get(pos.first, pos.second) = player2tag(player);
                int ret = evalPos(pos);
                this->all_board.get(pos.first, pos.second) = TA::BoardInterface::Tag::None;
                return ret;
            }
            std::cout << "minmax pos is not None\n";
            throw "error";
        }

        if (player)
        {
            if (this->all_board.get(pos.first, pos.second) == TA::BoardInterface::Tag::None)
                this->all_board.get(pos.first, pos.second) = player2tag(player);
            else
            {
                std::cout << "minmax pos is not None\n";
                throw "error";
            }

            int eval, maxEval = -INF;

            std::vector<std::pair<int, int>> enemyMoves;
            possibleEnemyMove(pos, enemyMoves);
            for (std::pair<int, int> pos : enemyMoves)
            {
                eval = minmax(pos, depth - 1, alpha, beta, !player);
                maxEval = std::max(maxEval, eval);
                alpha = std::max(alpha, eval);
                if (beta <= alpha)
                    break;
            }

            this->all_board.get(pos.first, pos.second) = TA::BoardInterface::Tag::None;
            return maxEval;
        }
        else
        {
            if (this->all_board.get(pos.first, pos.second) == TA::BoardInterface::Tag::None)
                this->all_board.get(pos.first, pos.second) = player2tag(player);
            else
            {
                std::cout << "minmax pos is not None\n";
                throw "error";
            }

            int eval, minEval = INF;

            std::vector<std::pair<int, int>> enemyMoves;
            possibleEnemyMove(pos, enemyMoves);
            for (std::pair<int, int> pos : enemyMoves)
            {
                eval = minmax(pos, depth - 1, alpha, beta, !player);
                minEval = std::min(minEval, eval);
                beta = std::min(beta, eval);
                if (beta <= alpha)
                    break;
            }

            this->all_board.get(pos.first, pos.second) = TA::BoardInterface::Tag::None;
            return minEval;
        }
    }

    int evalPos(std::pair<int, int> &pos)
    {
        // return the evaluation of pos
        return pos.first - pos.second;
    }

    void possibleEnemyMove(const std::pair<int, int> &pos, std::vector<std::pair<int, int>> &moves)
    {
        int subboard_x = pos.first % 3;
        int subboard_y = pos.second % 3;
        TA::Board subboard = this->all_board.sub(subboard_x, subboard_y);

        if (!subboard.full())
        {
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    if (subboard.state(i, j) == TA::BoardInterface::Tag::None)
                        moves.emplace_back(std::make_pair(i + subboard_x * 3, j + subboard_y * 3));
        }
        else
        {
            for (subboard_x = 0; subboard_x < 3; subboard_x++)
                for (subboard_y = 0; subboard_y < 3; subboard_y++)
                    for (int i = 0; i < 3; i++)
                        for (int j = 0; j < 3; j++)
                            if (subboard.state(i, j) == TA::BoardInterface::Tag::None)
                                moves.emplace_back(std::make_pair(i + subboard_x * 3, j + subboard_y * 3));
        }
    }
    // can only find the best step of given sub board
};
