#include "registers.h"
#include "util.h"

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>

#include <nlohmann/json.hpp>

using namespace std;
using namespace ftxui;

Registers::Registers(const string host) {
  m_host = host;

  initRegisters();
}

Registers::~Registers() {}

vector<vector<string>> Registers::getState() { return m_state; }

void Registers::updateState() {
  m_state.clear();
  m_state.push_back({{"Registers", "Hexa Value", "Value"}});

  for (string rg : m_registers) {
    string result = rtecli(m_host, format("registers -r {} get", rg));

    result.erase(0, 2);
    result.erase(result.size() - 3);

    // NOTE: We have no way to determine unsigned or signed
    // Currently we always consider unsigned
    m_state.push_back({rg, result, hexa2unsigned(result)});
  }
}

void Registers::clearRegisters() {
  for (string rg : m_registers) {
    rtecli(m_host, format("registers -r {} clear", rg));
  }
}

void Registers::initRegisters() {
  nlohmann::json json = rtecliJSON(m_host, "registers list");

  for (auto &element : json) {
    // NOTE: Currently we ignore registers with count > 1
    if (element["count"] == 1) {
      m_registers.push_back(element["name"]);
    }
  }

  updateState();
}
