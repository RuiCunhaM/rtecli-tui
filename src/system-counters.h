#ifndef SYSTEM_COUNTERS_H
#define SYSTEM_COUNTERS_H

#include "components/tab.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/node.hpp>
#include <string>
#include <vector>

class SystemCounters : public Tab {

public:
  SystemCounters(const std::string name, const std::string host);
  ~SystemCounters();

  void updateState();
  void clearSysCounters();
  ftxui::Component render();
  bool handleEvent(ftxui::Event event);

private:
  std::vector<std::vector<std::string>> m_state;
  std::vector<std::vector<std::string>> repr();
};

#endif
