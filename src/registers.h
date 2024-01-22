#ifndef REGISTERS_H
#define REGISTERS_H

#include <string>
#include <vector>

#include <ftxui/dom/node.hpp>

class Registers {

public:
  Registers(const std::string host);
  ~Registers();

  std::vector<std::vector<std::string>> getState();
  void updateState();
  void clearRegisters();

private:
  std::string m_host;
  std::vector<std::string> m_registers;
  std::vector<std::vector<std::string>> m_state;

  void initRegisters();
};

#endif
