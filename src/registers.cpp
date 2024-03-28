#include "registers.h"
#include "util.h"

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>

#include <nlohmann/json.hpp>

using namespace std;
using namespace ftxui;

Registers::Registers(const string name, const string host) : Tab(name, host) {
  initRegisters();
}

Registers::~Registers() {}

vector<vector<string>> Registers::repr() { return m_state; }

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

Component Registers::render() {
  auto reg_table = Renderer([this]() {
    Table t = Table(this->repr());
    t.SelectAll().Separator(LIGHT);
    styleTable(&t);
    return t.Render();
  });

  return Container::Horizontal({
      reg_table,
      Button("Clear Registers", [this] { this->clearRegisters(); }) |
          size(HEIGHT, EQUAL, 1),
  });
}

void Registers::handleEvent(Event event){};