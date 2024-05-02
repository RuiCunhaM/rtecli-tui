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

vector<vector<string>> SystemCounters::repr() {
  lock_guard<mutex> lock(m_mutex);
  return m_state;
}

void SystemCounters::updateState() {
  vector<vector<string>> new_state;
  new_state.push_back({"System Counters", "Value"});

  nlohmann::json json = rtecliJSON(m_host, "counters list-system");

  for (auto &element : json) {
    new_state.push_back({element["name"], to_string(element["value"])});
  }

  lock_guard<mutex> lock(m_mutex);
  m_state = new_state;
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

bool SystemCounters::handleEvent(Event event) { return false; };
