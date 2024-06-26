#ifndef SCROLLABLETAB_H
#define SCROLLABLETAB_H

#include "tab.h"
#include <string>
#include <vector>

#include <ftxui/component/component.hpp>
#include <ftxui/dom/node.hpp>

class ScrollableTab : public Tab {

public:
  ScrollableTab(const std::string name, const std::string host);
  ~ScrollableTab();
  bool handleEvent(ftxui::Event event);

protected:
  std::vector<std::vector<std::string>>
  reprTable(std::vector<std::vector<std::string>> table);

private:
  int m_rows;
  int m_offset;
  int m_tableSize;

  void ComputeNrows();
};

#endif
