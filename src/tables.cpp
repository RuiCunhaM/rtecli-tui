#include "tables.h"
#include "nlohmann/json_fwd.hpp"
#include "util.h"

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>

#include <nlohmann/json.hpp>

using namespace std;
using namespace ftxui;

Tables::Tables(const string name, const string host) : Tab(name, host) {
  initTables();
}

Tables::~Tables() {}

void Tables::updateState() {
  string table_selected = m_table_names[m_table_selected];

  nlohmann::json json =
      rtecliJSON(m_host, format("tables -t {} list-rules", table_selected));

  vector<vector<string>> new_state;
  new_state.push_back({"Rule Name", "Match", "Action", "Action Data",
                       "Priority", "Timeout (s)"});

  for (auto &elem : json) {

    string matchValues = "";
    for (auto &elem2 : elem["match"].items()) {
      matchValues.append(elem2.key());
      matchValues.append(": ");
      matchValues.append(elem2.value()["value"]);
      matchValues.append(" | ");
    }
    if (matchValues.size() > 3)
      matchValues.erase(matchValues.size() - 3);

    string dataValues;
    for (auto &elem3 : elem["actions"]["data"].items()) {
      dataValues.append(elem3.key());
      dataValues.append(": ");
      dataValues.append(elem3.value()["value"]);
      dataValues.append(" | ");
    }
    if (dataValues.size() >= 3)
      dataValues.erase(dataValues.size() - 3);

    new_state.push_back({
        elem["rule_name"],
        matchValues,
        elem["actions"]["type"],
        dataValues.length() ? dataValues : "None",
        to_string(elem["priority"]),
        to_string(elem["timeout_seconds"]),
    });
  }

  lock_guard<mutex> lock(m_mutex);
  m_state[table_selected] = new_state;
}

vector<vector<string>> Tables::repr() {
  lock_guard<mutex> lock(m_mutex);
  return m_state[m_table_names[m_table_selected]];
}

void Tables::initTables() {
  nlohmann::json json = rtecliJSON(m_host, "tables list");

  m_table_selected = 0;
  for (auto &elem : json) {
    string table = elem["tbl_name"];
    m_table_names.push_back(table);
    m_state[table] = {};
    updateState();
    m_table_selected++;
  }
  m_table_selected = 0;
}

Component Tables::render() {
  auto table_drop = Dropdown(&(this->m_table_names), &(this->m_table_selected));
  auto table_render = Renderer([this]() {
    Table t = Table(this->repr());
    t.SelectAll().Separator(LIGHT);
    styleTable(&t);
    return t.Render();
  });
  return Container::Vertical({
      table_drop,
      table_render,
  });
}

bool Tables::handleEvent(Event event) { return false; };
