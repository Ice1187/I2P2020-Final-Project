#pragma once

#include <UltraOOXX/Wrapper/AI.h>
#include <UltraOOXX/UltraBoard.h>
#include <algorithm>
#include <random>
#include <ctime>

#define INF 2147483647

class MinMaxAI : public AIInterface
{
private:
    int board_x, board_y;

public:
    void init(bool order) override
    {
        // any way
        this->board_x = this->board_y = 0;
    }

    void callbackReportEnemy(int x, int y) override
    {
        (void)x;
        (void)y;
        // give last step
        this->board_x = x / 3;
        this->board_y = y / 3;
    }

    std::pair<int, int> queryWhereToPut(TA::UltraBoard main_board) override
    {
        std::pair<int, int> step(-1, -1);
        TA::Board sub_board;

        sub_board = main_board.sub(this->board_x, this->board_y);
        if (!(sub_board.full()))
        {
            step = this->findStep(sub_board);
            step.first += board_x * 3;
            step.second += board_y * 3;
        }
        else
        {
            // for (int i = 0; i < 3; i++)
            //     for (int j = 0; j < 3; j++)
            //     {
            //         sub_board = main_board.sub(i, j);
            //         if (!(sub_board.full()))
            //         {
            //             step = findStep(sub_board);
            //             step.first += i * 3;
            //             step.second += j * 3;
            //         }
            //     }
        }
    }

    // can only find the best step of given sub board
    std::pair<int, int> findStep(TA::Board board)
    {
        int eval, maxEval = -INF;
        std::pair<int, int> cur_step, best_step;
        for (int i = 0; i < 3; i++)
        {
            cur_step.first = i;
            for (int j = 0; j < 3; j++)
            {
                if (board.state(i, j) == TA::BoardInterface::Tag::None)
                {
                    cur_step.second = j;
                    eval = minmax(cur_step, 3, 1);
                    if (eval > maxEval)
                    {
                        best_step = cur_step;
                        maxEval = eval;
                    }
                }
            }
        }

        return best_step;
    }

    int minmax(std::pair<int, int> &pos, int depth, bool player)
    {
        if (depth == 0)
            return evalPos(pos);

        if (player)
        {
            int eval, maxEval = -INF;

            std::vector<std::pair<int, int>> enemyMoves;
            possibleEnemyMove(enemyMoves);
            for (std::pair<int, int> pos : enemyMoves)
            {
                eval = minmax(pos, depth - 1, !player);
                maxEval = std::max(maxEval, eval);
            }

            return maxEval;
        }
        else
        {
            int eval, minEval = INF;

            std::vector<std::pair<int, int>> enemyMoves;
            possibleEnemyMove(enemyMoves);
            for (std::pair<int, int> pos : enemyMoves)
            {
                eval = minmax(pos, depth - 1, !player);
                minEval = std::max(minEval, eval);
            }

            return minEval;
        }
    }

    int evalPos(std::pair<int, int> &pos)
    {
        // return the evaluation of pos
    }

    void possibleEnemyMove(std::vector<std::pair<int, int>> &moves)
    {
        moves.push_back(std::make_pair(1, 2));
        moves.push_back(std::make_pair(3, 4));
        moves.push_back(std::make_pair(5, 6));
    }
};