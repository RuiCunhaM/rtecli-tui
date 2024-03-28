#ifndef REGISTERS_H
#define REGISTERS_H

#include <string>
#include <vector>

#include <ftxui/component/component.hpp>
#include <ftxui/dom/node.hpp>

#include "components/tab.h"

class Registers : public Tab {

public:
  Registers(const std::string name, const std::string host);
  ~Registers();

  void updateState();
  void clearRegisters();
  ftxui::Component render();
  void handleEvent(ftxui::Event event);

private:
  std::vector<std::string> m_registers;
  std::vector<std::vector<std::string>> m_state;

  std::vector<std::vector<std::string>> repr();
  void initRegisters();
};

#endif
