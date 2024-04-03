#include "scrollabletab.h"

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>

using namespace std;
using namespace ftxui;

ScrollableTab::ScrollableTab (const string name, const string host,
                              const int rows)
    : Tab (name, host)
{
  m_rows = rows;
  m_offset = 0;
}

ScrollableTab::~ScrollableTab () {}

void
ScrollableTab::handleEvent (Event event)
{
  if (event == Event::PageDown)
    {
      m_offset++;
    }
  else if (event == Event::PageUp)
    {
      m_offset--;
      if (m_offset < 0)
        m_offset = 0;
    }
};

vector<vector<string>> ScrollableTab::reprTable(vector<vector<string>> table) {
    vector<vector<string>> result;
    result.reserve(min(m_rows + 1,(int)table.size())); // Reserve memory for result

    // Headers
    vector<string> header = { "Index" };
    header.insert(header.end(), table[0].begin(), table[0].end());
    result.push_back(std::move(header)); // Move headers into result

    // Calculate start and end indices
    m_offset = m_offset % table.size();
    int start = 1 + m_offset;
    int end = min(start + m_rows - 1, (int)table.size() - 1);

    // Add rows to result
    for (int row = start; row <= end; row++) {
        vector<string> r = { to_string(row) };
        r.insert(r.end(), table[row].begin(), table[row].end());
        result.push_back(std::move(r)); // Move row into result
    }

    // Add empty rows if needed
    int rowsUsed = end - start + 1;
    for (int row = rowsUsed + 1; row <= min(m_rows,(int)table.size()-1); row++) {
        vector<string> r = { to_string(row-rowsUsed) };
        r.insert(r.end(), table[row-rowsUsed].begin(), table[row-rowsUsed].end());
        result.push_back(std::move(r)); // Move empty row into result
    }

    return result;
}

