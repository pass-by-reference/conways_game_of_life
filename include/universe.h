#ifndef universe_h_
#define universe_h_

#include <vector>
#include <string>

class Universe {
public:
Universe(
  const int rows,
  const int cols,
  const std::vector<std::pair<int, int>>& initial_state
);

void run();

private:

std::string get_str_universe();
void print_universe();
int get_live_neighbor_cells(int row, int col);
char handle_live_cell(int row, int col);
char handle_dead_cell(int row, int col);
void step_generation();


const int m_rows;
const int m_cols;
const std::vector<std::pair<int, int>>& m_initial_state;

int m_generation = 0;
std::vector<std::vector<char>> m_universe;
};

#endif