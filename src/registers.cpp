#include "registers.h"
#include "util.h"

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>

#include <nlohmann/json.hpp>

using namespace std;
using namespace ftxui;

Registers::Registers(const string name, const string host, const int rows)
    : ScrollableTab(name, host, rows) {
  initRegisters();
}

Registers::~Registers() {}

vector<vector<string>> Registers::repr() {
  return m_state[m_registers[m_register_selected]];
}

void Registers::updateState() {
  if (!m_registers.empty()) {
    string registerSelected = m_registers[m_register_selected];
    m_state[registerSelected].clear();
    m_state[registerSelected].push_back({"Registers", "Hexa Value", "Value"});

    nlohmann::json json =
        rtecliJSON(m_host, format("registers -r {} get", registerSelected));
    int i = 0;
    for (auto &element : json) {
      string ans = to_string(element);
      ans.erase(0, 1);
      ans.erase(ans.size() - 1);
      // NOTE: We have no way to determine unsigned or signed
      // Currently we always consider unsigned
      m_state[registerSelected].push_back(
          {to_string(i), ans, hexa2unsigned(ans)});
      i++;
    }
  }
}

void Registers::clearRegister() {
  if (!m_registers.empty()) {
    rtecli(m_host,
           format("registers -r {} clear", m_registers[m_register_selected]));
  }
}

void Registers::initRegisters() {
  nlohmann::json json = rtecliJSON(m_host, "registers list");
  m_register_selected = 0;

  for (auto &element : json) {
    m_registers.push_back(element["name"]);
    m_state[element["name"]] = {};
    updateState();
    m_register_selected++;
  }
  m_register_selected--;
}

Component Registers::render() {
  auto registers_drop =
      Dropdown(&(this->m_registers), &(this->m_register_selected));
  auto registers_render = Renderer([this]() {
    Table t = Table(this->reprTable(this->repr()));
    t.SelectAll().Separator(LIGHT);
    styleTable(&t);
    return t.Render();
  });
  auto dataContainer = Container::Vertical({
      registers_drop,
      registers_render,
  });

  return Container::Horizontal({
      dataContainer,
      Button("Clear Register", [this] { this->clearRegister(); }) |
          size(HEIGHT, EQUAL, 1),
  });
}
