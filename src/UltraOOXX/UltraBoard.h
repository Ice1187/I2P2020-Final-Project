#pragma once

#include <UltraOOXX/Board.h>
#include <iostream>
namespace TA {
    class UltraBoard : public BoardInterface {
    public:
        UltraBoard() {
            reset();
        }

        void reset() {
            for (int i=0;i<3;++i)
                for (int j=0;j<3;++j)
                    b[i][j].reset();
        }

        Board::Tag& get(int x, int y) {
            return b[x/3][y/3].get(x%3, y%3);
        }

        Board& sub(int x, int y) {
            return b[x][y];
        }

        Tag state(int x, int y) const override {
            return b[x][y].getWinTag();
        };

    private:
        Board b[3][3];
    };
} // Namespace TA
