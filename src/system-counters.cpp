#include "system-counters.h"
#include "util.h"

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>

#include <nlohmann/json.hpp>

using namespace std;
using namespace ftxui;

SystemCounters::SystemCounters(const string host) {
  m_host = host;

  updateState();
}

SystemCounters::~SystemCounters() {}

vector<vector<string>> SystemCounters::getState() { return m_state; }

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
