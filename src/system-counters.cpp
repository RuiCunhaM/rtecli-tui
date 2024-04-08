#include "system-counters.h"
#include "util.h"

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>

#include <nlohmann/json.hpp>

using namespace std;
using namespace ftxui;

SystemCounters::SystemCounters(const string name, const string host)
    : Tab(name, host) {
  updateState();
}

SystemCounters::~SystemCounters() {}

vector<vector<string>> SystemCounters::repr() { return m_state; }

void SystemCounters::updateState() {
  m_state.clear();
  m_state.push_back({"System Counters", "Value"});

  nlohmann::json json = rtecliJSON(m_host, "counters list-system");

  for (auto &element : json) {
    m_state.push_back({element["name"], to_string(element["value"])});
  }
}

void SystemCounters::clearSysCounters() {
  rtecli(m_host, "counters clear-all-system");
}

Component SystemCounters::render() {
  auto sysCnt_table = Renderer([this]() {
    Table t = Table(this->repr());
    styleTable(&t);
    return t.Render();
  });

  return Container::Horizontal({
      sysCnt_table,
      Button("Clear System Counters", [this] { this->clearSysCounters(); }) |
          size(HEIGHT, EQUAL, 1),
  });
}

void SystemCounters::handleEvent(Event event){};