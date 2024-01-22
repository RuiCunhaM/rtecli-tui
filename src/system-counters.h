#ifndef SYSTEM_COUNTERS_H
#define SYSTEM_COUNTERS_H

#include <string>
#include <vector>

#include <ftxui/dom/node.hpp>

class SystemCounters {

public:
  SystemCounters(const std::string host);
  ~SystemCounters();

  std::vector<std::vector<std::string>> getState();
  void updateState();
  void clearSysCounters();

private:
  std::string m_host;
  std::vector<std::vector<std::string>> m_state;
};

#endif
