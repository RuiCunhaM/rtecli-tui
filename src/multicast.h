#ifndef MULTICAST_H
#define MULTICAST_H

#include <string>
#include <vector>

#include "components/tab.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/node.hpp>

class MulticastGroups : public Tab {

public:
  MulticastGroups(const std::string name, const std::string host);
  ~MulticastGroups();

  void updateState();
  ftxui::Component render();
  bool handleEvent(ftxui::Event event);

private:
  int m_group_selected;
  std::vector<std::string> m_group_names;

  std::vector<std::vector<std::string>> m_state;
  std::vector<std::vector<std::string>> repr();
  std::vector<std::string> getGroupsNumber();
  void initMulticastGroups();
};

#endif
