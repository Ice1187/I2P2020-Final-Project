#pragma once

#include <UltraOOXX/Wrapper/AI.h>
#include <UltraOOXX/UltraBoard.h>
#include <useful/boardUtils.h>
#include <algorithm>
#include <random>
#include <ctime>

#define INF 2147483647

class MinMaxAI : public AIInterface
{
private:
    int prev_x, prev_y;
    bool order;
    TA::UltraBoard all_board;

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
        // any way
        this->prev_x = this->prev_y = 0;
        this->order = order;
        srand(time(nullptr));
    }

    void callbackReportEnemy(int x, int y) override
    {
        (void)x;
        (void)y;
        // give last step
        this->prev_x = x;
        this->prev_y = y;
    }

    std::pair<int, int> queryWhereToPut(TA::UltraBoard main_board) override
    {
        this->all_board = main_board;
        int subboard_x = this->prev_x % 3;
        int subboard_y = this->prev_y % 3;
        TA::Board subboard = this->all_board.sub(subboard_x, subboard_y);

        int eval = 0;
        int minEval = INF;
        int maxEval = -INF;
        std::pair<int, int> cur_step(-1, -1), best_step(-1, -1);

        if (!subboard.full())
        {
            for (int i = 0; i < 3; i++)
            {
                cur_step.first = i + subboard_x * 3;
                for (int j = 0; j < 3; j++)
                {
                    if (subboard.state(i, j) == TA::BoardInterface::Tag::None)
                    {
                        cur_step.second = j + subboard_y * 3;
                        eval = minmax(cur_step, 3, -INF, INF, this->order);
                        if (this->order == true && eval > maxEval)
                        {
                            best_step = cur_step;
                            maxEval = eval;
                        }
                        else if (this->order == false && eval < minEval)
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
            for (subboard_x = 0; subboard_x < 3; subboard_x++)
                for (subboard_y = 0; subboard_y < 3; subboard_y++)
                {
                    subboard = this->all_board.sub(subboard_x, subboard_y);
                    for (int i = 0; i < 3; i++)
                    {
                        cur_step.first = i + subboard_x * 3;
                        for (int j = 0; j < 3; j++)
                        {
                            if (subboard.state(i, j) == TA::BoardInterface::Tag::None)
                            {
                                cur_step.second = j + subboard_y * 3;
                                eval = minmax(cur_step, 3, -INF, INF, this->order);
                                if (this->order == true && eval > maxEval)
                                {
                                    best_step = cur_step;
                                    maxEval = eval;
                                }
                                else if (this->order == false && eval < minEval)
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
                int ret = evalPos();
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

            int eval = 0;
            int maxEval = -INF;

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

            int eval = 0;
            int minEval = INF;

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

    int evalPos()
    {
        // return the evaluation of pos
        return 1;
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
                {
                    subboard = this->all_board.sub(subboard_x, subboard_y);
                    for (int i = 0; i < 3; i++)
                        for (int j = 0; j < 3; j++)
                            if (subboard.state(i, j) == TA::BoardInterface::Tag::None)
                                moves.emplace_back(std::make_pair(i + subboard_x * 3, j + subboard_y * 3));
                }
        }
    }
};