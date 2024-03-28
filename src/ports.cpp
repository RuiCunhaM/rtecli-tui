#include "ports.h"

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <nlohmann/json.hpp>

#include "util.h"

using namespace std;
using namespace ftxui;

Ports::Ports(const string host) {
  m_host = host;

  updateState();
}

Ports::~Ports() {}

vector<vector<string>> Ports::getState() {
  vector<vector<string>> r;
  lock_guard<mutex> lock(m_mutex);
  r = m_state;
  return r;
}

void Ports::updateState() {
  vector<vector<string>> new_state;
  new_state.push_back({"Name", "Id", "Id hex", "Info"});

  nlohmann::json json = rtecliJSON(m_host, "ports list");

  for (auto &element : json) {
    auto id = to_string(element["id"]);
    new_state.push_back(
        {element["token"], id, unsigned2hexa(id), element["info"]});
  }

  lock_guard<mutex> lock(m_mutex);
  m_state = new_state;
}
