#pragma once

namespace TA {
    class BoardInterface {
    public:
        enum class Tag {
            None,
            O,
            X,
            Tie
        };

        virtual Tag state(int x, int y) const = 0;

        Tag getWinTag() const {return wintag;}
        void setWinTag(Tag t) {wintag = t;}
    private:
        Tag wintag;
    };


    

    class Board : public BoardInterface {
    public:
        Board() {
            reset();
        }

        void reset() {
            setWinTag(BoardInterface::Tag::None);
            for (int i=0;i<3;++i)
                for (int j=0;j<3;++j)
                    b[i][j] = Tag::None;
        }

        bool full() const {
            for (int i=0;i<3;++i)
                for (int j=0;j<3;++j)
                    if (b[i][j] == Tag::None)
                        return false;
            return true;
        }

        Tag state(int x, int y) const override {
            return b[x][y];
        };

        Tag& get(int x, int y) {
            return b[x][y];
        }

    private:
        Tag b[3][3];
    };
} // Namespace TA
