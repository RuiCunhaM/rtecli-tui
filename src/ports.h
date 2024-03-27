#ifndef PORTS_H
#define PORTS_H

#include <ftxui/dom/node.hpp>
#include <string>
#include <vector>

class Ports {
 public:
  Ports(const std::string host);
  ~Ports();

  std::vector<std::vector<std::string>> getState();
  void updateState();

 private:
  std::string m_host;
  std::vector<std::vector<std::string>> m_state;
};

#endif
