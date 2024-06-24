#ifndef PORTS_H
#define PORTS_H

#include "components/scrollabletab.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/node.hpp>
#include <string>
#include <vector>

class Ports : public ScrollableTab {
public:
  Ports(const std::string name, const std::string host);
  ~Ports();

  void updateState();
  ftxui::Component render();

private:
  std::vector<std::vector<std::string>> m_state;
  std::vector<std::vector<std::string>> repr();
  void Initialize();
};

#endif
