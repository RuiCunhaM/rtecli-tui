#include "scrollabletab.h"

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>

using namespace std;
using namespace ftxui;

ScrollableTab::ScrollableTab(const string name, const string host)
    : Tab(name, host) {
  m_offset = 0;
  ComputeNrows();
}

ScrollableTab::~ScrollableTab() {}

void ScrollableTab::ComputeNrows() {
  // NOTE: This -10 hardcoded value is not perfect. Some tabs
  // have less available space due to the dropdown menus.
  // Expanding these menus will mess up the tables underneath.
  // To fix this entirely, we need to determine at all times the exact
  // available vertical space.
  m_rows = (Terminal::Size().dimy - 10) / 2;
}

bool ScrollableTab::handleEvent(Event event) {
  if (event == Event::PageDown) {
    m_offset++;
    return true;
  } else if (event == Event::PageUp) {
    m_offset--;
    if (m_offset < 0)
      m_offset = 0;
    return true;
  } else if (event == Event::Home) {
    m_offset = 0;
    return true;
  } else if (event == Event::End) {
    m_offset = max(m_tableSize - m_rows - 1, 0);
    return true;
  }
  ComputeNrows();
  return false;
};

vector<vector<string>> ScrollableTab::reprTable(vector<vector<string>> table) {
  m_tableSize = (int)table.size();
  vector<vector<string>> result;
  result.reserve(min(m_rows + 1, m_tableSize)); // Reserve memory for result

  // Headers
  vector<string> header = {"Index"};
  header.insert(header.end(), table[0].begin(), table[0].end());
  result.push_back(std::move(header)); // Move headers into result

  // Calculate start and end indices
  m_offset = m_offset % m_tableSize;
  int start = 1 + m_offset;
  int end = min(start + m_rows - 1, m_tableSize - 1);

  // Add rows to result
  for (int row = start; row <= end; row++) {
    vector<string> r = {to_string(row - 1)};
    r.insert(r.end(), table[row].begin(), table[row].end());
    result.push_back(std::move(r)); // Move row into result
  }

  // Add empty rows if needed
  int rowsUsed = end - start + 1;
  for (int row = rowsUsed + 1; row <= min(m_rows, m_tableSize - 1); row++) {
    vector<string> r = {to_string(row - rowsUsed)};
    r.insert(r.end(), table[row - rowsUsed].begin(),
             table[row - rowsUsed].end());
    result.push_back(std::move(r)); // Move empty row into result
  }

  return result;
}
