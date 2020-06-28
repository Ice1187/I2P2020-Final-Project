#pragma once

#include <UltraOOXX/Wrapper/AI.h>
#include <UltraOOXX/UltraBoard.h>
#include <UltraOOXX/Game.h>
#include <useful/BoardUtils.h>
#include <algorithm>
#include <random>
#include <ctime>
#include <map>

#define INF 2147483647

class MinMaxAI : public AIInterface
{
private:
    using Tag = TA::BoardInterface::Tag;
    std::map<Tag, const int> tag2num = {
        {Tag::O, 1},
        {Tag::X, -1},
        {Tag::Tie, 0},
        {Tag::None, 0},
    };
    enum
    {
        O_LINE,
        X_LINE,
        O_BLOCKED,
        X_BLOCKED,
        O_TWO,
        X_TWO,
        O_ONE,
        X_ONE,
        TIE,
        EMPTY_1O1X
    };
    const int w_corner = 3;
    const int w_side = 2;
    const int w_center = 4;

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
    // to move to game.h
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
        // std::cout << "\nput chess: (" << pos.first << ", " << pos.second << ") " << (player ? "O" : "X") << '\n';
        if (this->all_board.get(pos.first, pos.second) != Tag::None)
        {
            // std::cout << "minmax pos is not None\n";
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
        // std::cout << "rm  chess: (" << pos.first << ", " << pos.second << ") " << (player ? "O" : "X") << "\n\n";

        if (this->all_board.get(pos.first, pos.second) != player2tag(player))
        {
            // std::cout << "you don't own the chess, so you can't remove it\n";
            throw "error";
        }

        // update subboard wintag
        this->all_board.get(pos.first, pos.second) = Tag::None;
        if (removeWinTag(this->all_board.sub(pos.first / 3, pos.second / 3)))
            // add allboard wintag
            removeWinTag(this->all_board);
    }

    int line_cond(const Tag &t1, const Tag &t2, const Tag &t3)
    {
        const int &n1 = tag2num[t1];
        const int &n2 = tag2num[t2];
        const int &n3 = tag2num[t3];
        const int line_cond = n1 + n2 + n3;

        bool contain_tie = (t1 == Tag::Tie || t2 == Tag::Tie || t3 == Tag::Tie);

        // if contain tie
        if (contain_tie)
            return TIE;

        // three the same
        else if (line_cond == 3)
            return O_LINE;
        else if (line_cond == -3)
            return X_LINE;

        // two O/X one empty
        else if (line_cond == 2)
            return O_TWO;
        else if (line_cond == -2)
            return X_TWO;

        // block enemy
        else if (line_cond == 1 && (n1 != 0 && n2 != 0 && n3 != 0)) // blcok by order = 0;
            return O_BLOCKED;
        else if (line_cond == -1 && (n1 != 0 && n2 != 0 && n3 != 0))
            return X_BLOCKED;

        // one O/X two empty
        else if (line_cond == 1)
            return O_ONE;
        else if (line_cond == -1)
            return X_ONE;

        else if (line_cond == 0)
            return EMPTY_1O1X;

        return 0;
    }

    int evalAllboardLine(const Tag &t1, const Tag &t2, const Tag &t3)
    {
        const int cond = line_cond(t1, t2, t3);

        switch (cond)
        {
        case O_LINE:
            return 1200;
        case X_LINE:
            return -1200;
        case O_BLOCKED:
            return -1000;
        case X_BLOCKED:
            return 1000;
        case O_TWO:
            return 600;
        case X_TWO:
            return -600;
        case O_ONE:
            return 300;
        case X_ONE:
            return -300;
        case TIE:
        case EMPTY_1O1X:
            return 0;
        }
        return 0;
    }

    int evalSubboardLine(const Tag &t1, const Tag &t2, const Tag &t3)
    {
        const int cond = line_cond(t1, t2, t3);
        switch (cond)
        {
        case O_LINE:
            return 120;
        case X_LINE:
            return -120;
        case O_BLOCKED:
            return -100;
        case X_BLOCKED:
            return 100;
        case O_TWO:
            return 60;
        case X_TWO:
            return -60;
        case O_ONE:
            return 30;
        case X_ONE:
            return -30;
        case TIE:
        case EMPTY_1O1X:
            return 0;
        }
        return 0;
    }

    int evalSubboard(TA::BoardInterface &board)
    {
        int ret = 0;
        Tag tags[3][3];
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                tags[i][j] = board.state(i, j);
                if (i + j == 2 && i == 1)
                    ret += tag2num[tags[i][j]] * 4;
                else if (i + j == 2 || i + j == 0)
                    ret += tag2num[tags[i][j]] * 3;
                else if (i + j == 1 || i + j == 3)
                    ret += tag2num[tags[i][j]] * 2;
            }

        // horizontal, vertical
        for (int i = 0; i < 3; i++)
        {
            ret += evalSubboardLine(tags[i][0], tags[i][1], tags[i][2]);
            ret += evalSubboardLine(tags[0][i], tags[1][i], tags[2][i]);
        }

        // diagonal, subdiagonal
        ret += evalSubboardLine(tags[0][0], tags[1][1], tags[2][2]);
        ret += evalSubboardLine(tags[2][0], tags[1][1], tags[0][2]);

        return ret;
    }

public:
    void
    init(bool order) override
    {
        // any way
        this->prev_x = this->prev_y = 4;
        this->order = order;
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
        int depth = 5;
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
                        eval = minmax(cur_step, depth, -INF, INF, this->order);
                        // std::cout << "step (" << cur_step.first << ", " << cur_step.second << ")  val: " << eval << ")\n";
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
                                eval = minmax(cur_step, depth, -INF, INF, this->order);
                                // std::cout << "step (" << cur_step.first << ", " << cur_step.second << ")  val: " << eval << ")\n";
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
            // std::cout << "pos " << "(" << pos.first << ", " << pos.second << ") : " << ret << '\n';
            removeChess(pos, player);
            return ret;
        }

        if (player)
        {
            putChess(pos, player);

            int eval;
            int maxEval = -INF;

            std::vector<std::pair<int, int>> enemyMoves;
            possibleEnemyMove(pos, enemyMoves);
            for (std::pair<int, int> pos : enemyMoves)
            {
                eval = minmax(pos, depth - 1, alpha, beta, !player);
                // std::cout << "step (" << pos.first << ", " << pos.second << ")  val: " << eval << ")\n";

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
        int subboard_x;
        int subboard_y;
        int subboard_val[3][3] = {};
        Tag tags[3][3];

        for (subboard_x = 0; subboard_x < 3; subboard_x++)
            for (subboard_y = 0; subboard_y < 3; subboard_y++)
            {
                tags[subboard_x][subboard_y] = this->all_board.state(subboard_x, subboard_y);
                if (tags[subboard_x][subboard_y] == Tag::None)
                    subboard_val[subboard_x][subboard_y] = evalSubboard(this->all_board.sub(subboard_x, subboard_y));
                else
                    subboard_val[subboard_x][subboard_y] = tag2num[tags[subboard_x][subboard_y]] * 300;
            }

        // std::cout << "subboard val: \n";
        // for (int i = 0; i < 3; i++)
        // {
        // std::cout << "\t";
        // for (int j = 0; j < 3; j++)
        // std::cout << subboard_val[i][j] << " | ";
        // std::cout << "\n";
        // }

        int ret = 0;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                if (i + j == 2 && i == 1)
                    ret += subboard_val[i][j] / 6;
                else if (i + j == 2 || i + j == 0)
                    ret += subboard_val[i][j] / 8;
                else if (i + j == 1 || i + j == 3)
                    ret += subboard_val[i][j] / 12;
            }
        // std::cout << "subboard init ret: " << ret << '\n';

        // horizontal, vertical
        for (int i = 0; i < 3; i++)
        {
            ret += evalAllboardLine(tags[i][0], tags[i][1], tags[i][2]);
            ret += evalAllboardLine(tags[0][i], tags[1][i], tags[2][i]);
        }
        // std::cout << "subboard h/v ret: " << ret << '\n';

        // diagonal, subdiagonal
        ret += evalAllboardLine(tags[0][0], tags[1][1], tags[2][2]);
        ret += evalAllboardLine(tags[2][0], tags[1][1], tags[0][2]);

        // return the evaluation of pos
        return ret;
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