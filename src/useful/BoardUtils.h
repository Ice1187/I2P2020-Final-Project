#pragma once

#include <UltraOOXX/UltraBoard.h>
#include <UltraOOXX/Wrapper/AI.h>
#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>

using std::pair;

namespace US
{
  pair<int, int> getNextSubboardId(int x, int y)
  {
    return pair(x % 3, y % 3);
  }
  pair<int, int> getNowSubboardId(int x, int y)
  {
    return pair(x / 3, y / 3);
  }
  bool inSubboard(int pos_x, int pos_y, int board_x, int board_y)
  {
    return (board_x * 3 <= pos_x && pos_x < board_x * 3 + 3 &&
            board_y * 3 <= pos_y && pos_y < board_y * 3 + 3);
  }

} // namespace US