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
    using Tag = TA::BoardInterface::Tag;
    int prev_x, prev_y;
    bool order;
    TA::UltraBoard all_board;

    Tag player2tag(bool player)
    {
        if (player == true)
            return Tag::O;
        else
            return Tag::X;
    }

    bool addWinTag(const std::pair<int, int> &pos, TA::BoardInterface &board)
    {
        if (board.getWinTag() != Tag::None)
            return false;

        const int &pos_x = pos.first;
        const int &pos_y = pos.second;

        // horizontal
        if (board.state(pos_x, pos_y) == board.state((pos_x + 1) % 3, pos_y) &&
            board.state(pos_x, pos_y) == board.state((pos_x + 2) % 3, pos_y))
        {
            board.setWinTag(board.state(pos_x, pos_y));
            return true;
        }

        // vertical
        if (board.state(pos_x, pos_y) == board.state(pos_x, (pos_y + 1) % 3) &&
            board.state(pos_x, pos_y) == board.state(pos_x, (pos_y + 2) % 3))
        {
            board.setWinTag(board.state(pos_x, pos_y));
            return true;
        }

        // diagonal
        if (pos_x == pos_y &&
            board.state(pos_x, pos_y) == board.state((pos_x + 1) % 3, (pos_y + 1) % 3) &&
            board.state(pos_x, pos_y) == board.state((pos_x + 2) % 3, (pos_y + 2) % 3))
        {
            board.setWinTag(board.state(pos_x, pos_y));
            return true;
        }

        // subdiagonal
        if ((pos_x + pos_y) == 2 &&
            board.state(pos_x, pos_y) == board.state((pos_x + 2) % 3, (pos_y + 1) % 3) &&
            board.state(pos_x, pos_y) == board.state((pos_x + 1) % 3, (pos_y + 2) % 3))
        {
            board.setWinTag(board.state(pos_x, pos_y));
            return true;
        }

        return false;
    }
    bool addSubboardWinTag(const std::pair<int, int> &pos, TA::Board &board)
    {
        if (addWinTag(pos, board))
            return true;
        if (board.full())
            board.setWinTag(Tag::Tie);
        return false;
    }
    bool addAllboardWinTag(const std::pair<int, int> &pos, TA::UltraBoard &board)
    {
        if (addWinTag(pos, board))
            return true;
        return false;
    }

    bool removeWinTag(TA::BoardInterface &board)
    {
        if (board.getWinTag() == Tag::None)
            return false;
        if (board.getWinTag() == Tag::Tie)
        {
            board.setWinTag(Tag::None);
            return false;
        }

        // if win_lines >= 1,  wintag remain no change.
        int win_lines = 0;

        // horizontal
        for (int i = 0; i < 3; i++)
        {
            if (board.state(i, 0) == Tag::O &&
                board.state(i, 1) == Tag::O &&
                board.state(i, 2) == Tag::O)
                win_lines += 1;
            if (board.state(i, 0) == Tag::X &&
                board.state(i, 1) == Tag::X &&
                board.state(i, 2) == Tag::X)
                win_lines += 1;
        }

        // vertical
        for (int j = 0; j < 3; j++)
        {
            if (board.state(0, j) == Tag::O &&
                board.state(1, j) == Tag::O &&
                board.state(2, j) == Tag::O)
                win_lines += 1;
            if (board.state(0, j) == Tag::X &&
                board.state(1, j) == Tag::X &&
                board.state(2, j) == Tag::X)
                win_lines += 1;
        }

        // diagonal
        if (board.state(0, 0) == Tag::O &&
            board.state(1, 1) == Tag::O &&
            board.state(2, 2) == Tag::O)
            win_lines += 1;
        if (board.state(0, 0) == Tag::X &&
            board.state(1, 1) == Tag::X &&
            board.state(2, 2) == Tag::X)
            win_lines += 1;

        // subdiagonal
        if (board.state(0, 2) == Tag::O &&
            board.state(1, 1) == Tag::O &&
            board.state(2, 0) == Tag::O)
            win_lines += 1;
        if (board.state(0, 2) == Tag::X &&
            board.state(1, 1) == Tag::X &&
            board.state(2, 0) == Tag::X)
            win_lines += 1;

        if (win_lines == 0)
        {
            board.setWinTag(Tag::None);
            return true;
        }

        return false;
    }

    void putChess(const std::pair<int, int> &pos, bool player)
    {
        if (this->all_board.get(pos.first, pos.second) != Tag::None)
        {
            std::cout << "minmax pos is not None\n";
            throw "error";
        }

        this->all_board.get(pos.first, pos.second) = player2tag(player);
        // add subboard wintag
        if (addSubboardWinTag(std::make_pair(pos.first % 3, pos.second % 3), this->all_board.sub(pos.first / 3, pos.second / 3)))
            // add allboard wintag
            addAllboardWinTag(std::make_pair(pos.first / 3, pos.second / 3), this->all_board);
    }

    void removeChess(const std::pair<int, int> &pos, bool player)
    {
        if (this->all_board.get(pos.first, pos.second) != player2tag(player))
        {
            std::cout << "you don't own the chess, so you can't remove it\n";
            throw "error";
        }

        // update subboard wintag
        this->all_board.get(pos.first, pos.second) = Tag::None;
        if (removeWinTag(this->all_board.sub(pos.first / 3, pos.second / 3)))
            // add allboard wintag
            removeWinTag(this->all_board);
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
                    if (subboard.state(i, j) == Tag::None)
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
                            if (subboard.state(i, j) == Tag::None)
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
            putChess(pos, player);
            int ret = evalPos();
            removeChess(pos, player);
            return ret;
        }

        if (player)
        {
            putChess(pos, player);

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

            removeChess(pos, player);
            return maxEval;
        }
        else
        {
            putChess(pos, player);

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

            removeChess(pos, player);
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
                    if (subboard.state(i, j) == Tag::None)
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
                            if (subboard.state(i, j) == Tag::None)
                                moves.emplace_back(std::make_pair(i + subboard_x * 3, j + subboard_y * 3));
                }
        }
    }
};