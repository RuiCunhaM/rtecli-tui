#ifndef REGISTERS_H
#define REGISTERS_H

#include <map>
#include <string>
#include <vector>

#include <ftxui/component/component.hpp>
#include <ftxui/dom/node.hpp>

#include "components/scrollabletab.h"

class Registers : public ScrollableTab {

public:
  Registers(const std::string name, const std::string host);
  ~Registers();

  void updateState();
  void clearRegister();
  ftxui::Component render();

private:
  int m_register_selected;
  std::vector<std::string> m_registers;
  std::vector<std::string> m_single_registers;
  std::map<std::string, std::vector<std::vector<std::string>>> m_state;

  std::vector<std::vector<std::string>> repr();
  void initRegisters();
  std::mutex m_mutex;
};

#endif
