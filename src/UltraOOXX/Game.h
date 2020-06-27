#pragma once

#include <UltraOOXX/UltraBoard.h>
#include <UltraOOXX/Wrapper/AI.h>
#include <GUI/GUIInterface.h>
#include <useful/boardUtils.h>

#include <iostream>
#include <cassert>
#include <chrono>
#include <cstdarg>
#include <future>
#include <type_traits>
#include <algorithm>

// include AI
// #include "AITemplate/DummyAI.h"
#include "AITemplate/MinMaxAI.h"
namespace TA
{
    using Tag = BoardInterface::Tag;

    class UltraOOXX
    {
    public:
        UltraOOXX(
            std::chrono::milliseconds runtime_limit = std::chrono::milliseconds(1000)) : m_runtime_limit(runtime_limit),
                                                                                         m_P1(nullptr),
                                                                                         m_P2(nullptr),
                                                                                         MainBoard()
        {
            gui = new ASCII;
        }

        void setPlayer1(AIInterface *ptr)
        {
            assert(checkAI(ptr));
            m_P1 = ptr;
        }
        void setPlayer2(AIInterface *ptr)
        {
            assert(checkAI(ptr));
            m_P2 = ptr;
        }

        void run()
        {
            gui->title();
            if (!prepareState())
                return;

            // Game Start
            putToGui("[info] Game start\n");
            updateGuiGame();

            int round = 1;
            AIInterface *first = m_P1;
            AIInterface *second = m_P2;
            Tag tag = Tag::O;

            while (!checkGameover())
            {
                if (!playOneRound(first, tag, second))
                {
                    putToGui("[info] Fail to play this round!\n");
                    if (tag == Tag::O)
                        this->MainBoard.setWinTag(Tag::X);
                    else if (tag == Tag::X)
                        this->MainBoard.setWinTag(Tag::O);
                    break;
                }

                // render
                updateGuiGame();

                // switch player
                std::swap(first, second);
                tag = (tag == Tag::O) ? Tag::X : Tag::O;
                round++;
                putToGui("\n");
            }

            // Gameover
            putToGui("[info] Gameover!\n");
            switch (this->MainBoard.getWinTag())
            {
            case Tag::O:
                putToGui("[info] O win!\n");
                break;
            case Tag::X:
                putToGui("[info] X win!\n");
                break;
            case Tag::Tie:
                putToGui("[info] Tie!\n");
                break;
            default:
                putToGui("[info] Nobody win. There may be some bugs!\n");
                break;
            }
        }

    private:
        // self added function
        void updateBoardWinTag(BoardInterface &board)
        {
            if (board.getWinTag() != Tag::None)
                return;

            Tag tags[3][3];
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    tags[i][j] = board.state(i, j);

            bool O_win = false;
            bool X_win = false;
            // horizontal
            for (int i = 0; i < 3; i++)
            {
                if (O_win == false &&
                    tags[i][0] == Tag::O &&
                    tags[i][1] == Tag::O &&
                    tags[i][2] == Tag::O)
                    O_win = true;
                if (X_win == false &&
                    tags[i][0] == Tag::X &&
                    tags[i][1] == Tag::X &&
                    tags[i][2] == Tag::X)
                    X_win = true;
            }

            // vertical
            for (int j = 0; j < 3; j++)
            {
                if (O_win == false &&
                    tags[0][j] == Tag::O &&
                    tags[1][j] == Tag::O &&
                    tags[2][j] == Tag::O)
                    O_win = true;
                if (X_win == false &&
                    tags[0][j] == Tag::X &&
                    tags[1][j] == Tag::X &&
                    tags[2][j] == Tag::X)
                    X_win = true;
            }

            // diagonal
            if (O_win == false &&
                tags[0][0] == Tag::O &&
                tags[1][1] == Tag::O &&
                tags[2][2] == Tag::O)
                O_win = true;
            if (X_win == false &&
                tags[0][0] == Tag::X &&
                tags[1][1] == Tag::X &&
                tags[2][2] == Tag::X)
                X_win = true;
            // subdiagonal
            if (O_win == false &&
                tags[0][2] == Tag::O &&
                tags[1][1] == Tag::O &&
                tags[2][0] == Tag::O)
                O_win = true;
            if (X_win == false &&
                tags[0][2] == Tag::X &&
                tags[1][1] == Tag::X &&
                tags[2][0] == Tag::X)
                X_win = true;

            // set wintag of board
            if (O_win == true && X_win == true)
                board.setWinTag(Tag::Tie);
            else if (O_win == true)
                board.setWinTag(Tag::O);
            else if (X_win == true)
                board.setWinTag(Tag::X);
            else
                board.setWinTag(Tag::None);
        }

        bool isPutChessPosValid(std::pair<int, int> pos)
        {
            auto [nextSubboard_x, nextSubboard_y] = US::getNextSubboardId(this->prev_pos.first, this->prev_pos.second);
            auto [pos_x, pos_y] = pos;

            // check if pos follows the prev pos restrict rule
            if (this->prev_pos.first >= 0 && this->prev_pos.second >= 0)
                if (!this->MainBoard.sub(nextSubboard_x, nextSubboard_y).full())
                    if (!US::inSubboard(pos_x, pos_y, nextSubboard_x, nextSubboard_y))
                        return false;

            // check `pos` is empty
            if (this->MainBoard.get(pos_x, pos_y) != Tag::None)
                return false;

            return true;
        }

        bool playOneRound(AIInterface *user, Tag tag, AIInterface *enemy)
        {
            // `pos`: where user put the chess
            auto pos = call(&AIInterface::queryWhereToPut, user, MainBoard);

            // check whether `pos` is valid
            putToGui("put (%d, %d)\n");
            if (!isPutChessPosValid(pos))
                return false;

            // put the chess
            MainBoard.get(pos.first, pos.second) = tag;
            updateBoardWinTag(MainBoard.sub(pos.first / 3, pos.second / 3));

            // callback enemy
            enemy->callbackReportEnemy(pos.first, pos.second);
            // callback UltraOOXX
            this->prev_pos = pos;

            return true;
        }

        bool checkGameover()
        {
            // update main_board win tag
            updateBoardWinTag(this->MainBoard);

            if (MainBoard.getWinTag() != Tag::None)
                return true;
            return false;
        }

        void updateGuiGame()
        {
            gui->updateGame(MainBoard);
        }

        bool prepareState()
        {
            call(&AIInterface::init, m_P1, true);
            call(&AIInterface::init, m_P2, false);
            // doesn't init in UltraBoard.h
            this->MainBoard.setWinTag(Tag::None);
            this->prev_pos = std::make_pair(-1, -1);
            return true;
        }

        template <typename Func, typename... Args,
                  std::enable_if_t<std::is_void<
                                       std::invoke_result_t<Func, AIInterface, Args...>>::value,
                                   int> = 0>
        void call(Func func, AIInterface *ptr, Args... args)
        {
            std::future_status status;
            auto val = std::async(std::launch::async, func, ptr, args...);
            status = val.wait_for(std::chrono::milliseconds(m_runtime_limit));

            if (status != std::future_status::ready)
            {
                exit(-1);
            }
            val.get();
        }

        template <typename Func, typename... Args,
                  std::enable_if_t<std::is_void<
                                       std::invoke_result_t<Func, AIInterface, Args...>>::value == false,
                                   int> = 0>
        auto call(Func func, AIInterface *ptr, Args... args)
            -> std::invoke_result_t<Func, AIInterface, Args...>
        {
            std::future_status status;
            auto val = std::async(std::launch::async, func, ptr, args...);
            status = val.wait_for(std::chrono::milliseconds(m_runtime_limit));

            if (status != std::future_status::ready)
            {
                exit(-1);
            }
            return val.get();
        }

        void putToGui(const char *fmt, ...)
        {
            va_list args1;
            va_start(args1, fmt);
            va_list args2;
            va_copy(args2, args1);
            std::vector<char> buf(1 + std::vsnprintf(nullptr, 0, fmt, args1));
            va_end(args1);
            std::vsnprintf(buf.data(), buf.size(), fmt, args2);
            va_end(args2);

            if (buf.back() == 0)
                buf.pop_back();
            gui->appendText(std::string(buf.begin(), buf.end()));
        }

        bool checkAI(AIInterface *ptr)
        {
            return ptr->abi() == AI_ABI_VER;
        }

        // self added variables
        std::pair<int, int> prev_pos;

        int m_size;
        std::vector<int> m_ship_size;
        std::chrono::milliseconds m_runtime_limit;

        AIInterface *m_P1;
        AIInterface *m_P2;
        GUIInterface *gui;

        UltraBoard MainBoard;
    };
} // namespace TA
