#include "ports.h"

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <nlohmann/json.hpp>

#include "util.h"

using namespace std;
using namespace ftxui;

Ports::Ports(const string name, const string host, const int rows)
    : ScrollableTab(name, host, rows) {
  updateState();
}

Ports::~Ports() {}

vector<vector<string>> Ports::repr() { return m_state; }

void Ports::updateState() {
  m_state.clear();
  m_state.push_back({"Name", "Id", "Id hex", "Info"});

  nlohmann::json json = rtecliJSON(m_host, "ports list");

  for (auto &element : json) {
    auto id = to_string(element["id"]);
    m_state.push_back(
        {element["token"], id, unsigned2hexa(id), element["info"]});
  }
}

Component Ports::render() {
  return Renderer([this]() {
    Table t = Table(this->reprTable(this->repr()));
    t.SelectAll().Separator(LIGHT);
    styleTable(&t);
    return t.Render();
  });
}
