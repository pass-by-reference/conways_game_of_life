#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

#include "include/universe.h"
#include "src/constants.h"

Universe build_universe(const std::string& filename) {
  std::string line;
  std::ifstream file(filename);

  int row = 0;
  int col = 0;

  int COL_MAX = 0;
  int ROW_MAX = 0;
  std::vector<std::pair<int, int>> seed;

  if(file.is_open()) {
    while ( getline (file,line) )
    {
      for (const char ch : line) {
        if(ch == 'O') {
          seed.push_back(std::make_pair(row + constants::PADDING, col + constants::PADDING));
        }

        col++;
      }

      if(col > COL_MAX) {
        COL_MAX = col;
      }

      col = 0;
      row++;
    }

    ROW_MAX = row;
    file.close();

    return Universe(ROW_MAX + constants::PADDING * 2, COL_MAX + constants::PADDING * 2, seed);
  }

  throw std::runtime_error("No file " + filename);
}

int main (int argc, char *argv[]) {
  Universe univ = build_universe("patterns/gospher_gun_glider.txt");
  univ.run();

  return 0;
}
