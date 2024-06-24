#include "registers.h"
#include "util.h"

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>

#include <nlohmann/json.hpp>

using namespace std;
using namespace ftxui;

Registers::Registers(const string name, const string host)
    : ScrollableTab(name, host), m_register_selected(0) {
  initRegisters();
}

Registers::~Registers() {}

vector<vector<string>> Registers::repr() {
  lock_guard<mutex> lock(m_mutex);
  return m_state[m_registers[m_register_selected]];
}

void Registers::updateState() {
  if (m_registers.empty())
    return;

  string rgSelected = m_registers[m_register_selected];

  vector<vector<string>> new_state;

  if (rgSelected == "Single Registers") {
    new_state.push_back({"Registers", "Hexa Value", "Value"});
    for (string rg : m_single_registers) {
      string result = rtecli(m_host, format("registers -r {} get", rg));

      result.erase(0, 2);
      result.erase(result.size() - 3);

      new_state.push_back({rg, result, hexa2unsigned(result)});
    }
  } else {
    new_state.push_back({"Hexa Value", "Value"});
    nlohmann::json json =
        rtecliJSON(m_host, format("registers -r {} get", rgSelected));

    for (auto &element : json) {
      string ans = to_string(element);
      ans.erase(0, 1);
      ans.erase(ans.size() - 1);
      new_state.push_back({ans, hexa2unsigned(ans)});
    }
  }

  lock_guard<mutex> lock(m_mutex);
  m_state[rgSelected] = new_state;
}

void Registers::clearRegister() {
  if (m_registers.empty())
    return;

  string rgSelected = m_registers[m_register_selected];

  if (rgSelected == "Single Registers")
    for (string reg : m_single_registers)
      rtecli(m_host, format("registers -r {} clear", reg));
  else
    rtecli(m_host,
           format("registers -r {} clear", m_registers[m_register_selected]));
}

void Registers::initRegisters() {
  nlohmann::json json = rtecliJSON(m_host, "registers list");

  for (auto &element : json) {
    if (element["count"] == 1)
      m_single_registers.push_back(element["name"]);
    else {
      m_registers.push_back(element["name"]);
      m_state[element["name"]] = {};
    }
  }

  if (!m_single_registers.empty()) {
    m_registers.insert(m_registers.begin(), "Single Registers");
    m_state["Single Registers"] = {};
  }

  for (size_t i = 0; i < m_registers.size(); i++) {
    updateState();
    m_register_selected++;
  }
  m_register_selected = 0;
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
