#ifndef MULTICAST_H
#define MULTICAST_H

#include <map>
#include <string>
#include <tuple>
#include <vector>

#include <ftxui/dom/node.hpp>

class MulticastGroups {

public:
  MulticastGroups(const std::string host);
  ~MulticastGroups();

  std::vector<std::vector<std::string>> getState();
  void updateState();
  std::vector<std::string> getGroupsNumber();
  std::vector<std::vector<std::string>> getGroupState(int group_id);

private:
  std::string m_host;
  std::vector<std::vector<std::string>> m_state;

  void initMulticastGroups();
};

#endif
