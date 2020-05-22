#pragma once

#include <cstdio>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>

#include <UltraOOXX/UltraBoard.h>

namespace TA
{
    class GUIInterface
    {
    public:
        virtual void title() = 0;
        virtual void appendText(std::string str) = 0;
        virtual void updateGame(UltraBoard b) = 0;
    };

    using std::printf;
    #define ESC "\033"
    class ASCII: public GUIInterface
    {
        const static int GRAPH_HIGHT = 7+15;
        const static int GRAPH_WIDTH = 80;
        
        const static int TEXT_HIGHT = 10;
        const static int TEXT_WIDTH = 80;

        std::string m_preparedText;
        std::string m_textbuf;

        void cls()
        {
            printf( ESC "[H" ESC "[J" );
        }

        void gotoxy(int y, int x)
        {
            printf( ESC "\033[%d;%df", y, x);
        }

        void updateTextBuf()
        {
            std::stringstream ss(m_textbuf);
            const std::string ban(TEXT_WIDTH, '-');
            std::string tmp, last;

            m_preparedText.clear();
            for(int L=0 ; L < TEXT_HIGHT - 2; ++L)
            {
                if( last == "" )
                    getline(ss, last);

                tmp = last.substr(0, std::min((size_t)TEXT_WIDTH, last.size()) );
    
                if( tmp.size() == last.size() )
                    last = "";
                else 
                    last = last.substr(TEXT_WIDTH);
    
                m_preparedText = tmp + "\n" + m_preparedText;
            }
            m_textbuf = m_textbuf.substr(0, TEXT_HIGHT * TEXT_WIDTH);
            m_preparedText =  ban + "\n" + m_preparedText + ban;
        }

        void showText()
        {
            gotoxy(GRAPH_HIGHT+1, 0);
            printf( ESC "[J" );
            gotoxy(GRAPH_HIGHT+1, 0);
            puts(m_preparedText.c_str());
            gotoxy(GRAPH_HIGHT+TEXT_HIGHT+1, 0);
            std::fflush(stdout);
        }

    public:
        virtual void title() override
        {
            cls();
            puts(
R"( _   _ _ _             _____  _______   ____   __
| | | | | |           |  _  ||  _  \ \ / /\ \ / /
| | | | | |_ _ __ __ _| | | || | | |\ V /  \ V /
| | | | | __| '__/ _` | | | || | | |/   \  /   \
| |_| | | |_| | | (_| \ \_/ /\ \_/ / /^\ \/ /^\ \
 \___/|_|\__|_|  \__,_|\___/  \___/\/   \/\/   \/
)");
        }

        virtual void appendText(std::string str)
        {
            m_textbuf = str + m_textbuf;
            updateTextBuf();
            showText();
        }

        int toPrintChar(BoardInterface::Tag t){
            switch(t) {
                case BoardInterface::Tag::O: return 'O';
                case BoardInterface::Tag::X: return 'X';
                default:
                    return ' ';
            }
        }

        virtual void updateGame(UltraBoard b)
        {
            gotoxy(7+1, 0);
            const std::string buf(20, ' ');

            for (int i=0;i<9;++i)
            {
                std::printf("%s", buf.c_str());
                for (int j=0;j<9;++j)
                {
                    std::putchar(toPrintChar(b.get(i, j)));
                    if (j == 2 || j == 5) std::putchar('|');
                }
                std::putchar('\n');
                if (i==2 ||i==5) {
                    std::printf("%s", buf.c_str());
                    std::puts(std::string(12,'-').c_str());
                }
            }

            gotoxy(GRAPH_HIGHT+TEXT_HIGHT+1, 0);
        }
    };
    #undef ESC
}
