#include "universe.h"
#include "constants.h"

#include <curses.h>
#include <thread>
#include <iostream>
#include <algorithm>

Universe::Universe(
  const int rows,
  const int cols,
  const std::vector<std::pair<int, int>>& initial_state) : 
  m_rows{rows}, m_cols{cols}, m_initial_state{initial_state}
{

  m_universe.resize(m_rows);
  std::for_each(m_universe.begin(), m_universe.end(), [this](std::vector<char>& row) {
    row.resize(m_cols);
    std::fill(row.begin(), row.end(), constants::DEAD);
  });

  for(const std::pair<int,int>& coord : m_initial_state) {
    int row = coord.first;
    int col = coord.second;

    m_universe[row][col] = constants::LIVE;
  }

}

void Universe::run() {

  if(!constants::DEBUG) {
    initscr();
  }

  while (true)
  {
    step_generation();
    print_universe();
    m_generation++;
  }

  if(!constants::DEBUG) {
    endwin();
  }

}

// ================================

std::string Universe::get_str_universe() {
  std::string board_str = "";

  for(int row = 0; row < m_rows; row++) {
    if (row < constants::PADDING || row > m_rows - constants::PADDING) {
      continue;
    }

    for (int col = 0; col < m_cols; col++) {
      if (col < constants::PADDING || col > m_cols - constants::PADDING) {
        continue;
      }

      board_str += m_universe[row][col];
    }

    board_str += "\n";
  }

  return board_str;
}

void Universe::print_universe() {
  const std::string universe = get_str_universe();
  const std::string heading = "\nCurrent Generation: " + std::to_string(m_generation) + "\n";

  if(constants::DEBUG) {
    std::cout << heading << std::endl;
    std::cout << universe;
  } else {
    clear();
    addstr(heading.c_str());
    addstr(universe.c_str());
    refresh();
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(constants::REFRESH_MILLI));
}

int Universe::get_live_neighbor_cells(int row, int col) {
  int live_neighbor_cells = 0;

  const auto LIVE = constants::LIVE;

  if(m_universe[row-1][col-1] == LIVE) live_neighbor_cells += 1;
  if(m_universe[row-1][col] == LIVE) live_neighbor_cells += 1;
  if(m_universe[row-1][col+1] == LIVE) live_neighbor_cells += 1;
  if(m_universe[row][col-1] == LIVE) live_neighbor_cells += 1;
  if(m_universe[row][col+1] == LIVE) live_neighbor_cells += 1;
  if(m_universe[row+1][col-1] == LIVE) live_neighbor_cells += 1;
  if(m_universe[row+1][col] == LIVE) live_neighbor_cells += 1;
  if(m_universe[row+1][col+1] == LIVE) live_neighbor_cells += 1;

  return live_neighbor_cells;
}

char Universe::handle_live_cell(int row, int col) {
  int live_neighbor_cells = get_live_neighbor_cells(row, col);

  if(live_neighbor_cells == 2 || live_neighbor_cells == 3) {
    return constants::LIVE;
  } else {
    return constants::DEAD;
  }
}

char Universe::handle_dead_cell(int row, int col) {
  int live_neighbor_cells = get_live_neighbor_cells(row, col);

  if(live_neighbor_cells == 3) {
    return constants::LIVE;
  } else {
    return constants::DEAD;
  }
}

void Universe::step_generation() {
  auto next_gen_board = m_universe; // Copy

  // Loop through row and col but not the border cells
  for(int row = 1; row < m_rows - 1; row++) {
    for (int col = 1; col < m_cols - 1; col++) {

      if(m_universe[row][col] == constants::LIVE) {
        next_gen_board[row][col] = handle_live_cell(row, col);
      } else {
        next_gen_board[row][col] = handle_dead_cell(row, col);
      }
    }
  }

  m_universe = next_gen_board; // copy again.
}
