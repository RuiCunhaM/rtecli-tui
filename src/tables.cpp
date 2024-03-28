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

Tables::Tables(const string host) {
  m_host = host;

  initTables();
}

Tables::~Tables() {}

vector<string> Tables::getTables() { return m_tables; };

void Tables::updateTableState(const string tableName) {
  vector<vector<string>> new_state;
  new_state.push_back({"Rule Name", "Match", "Action", "Action Data",
                       "Priority", "Timeout (s)"});

  nlohmann::json json =
      rtecliJSON(m_host, format("tables -t {} list-rules", tableName));

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
  m_state[tableName] = new_state;
}

vector<vector<string>> Tables::getTableState(const string tableName) {
  vector<vector<string>> r;
  lock_guard<mutex> lock(m_mutex);
  r = m_state[tableName];
  return r;
}

void Tables::initTables() {
  nlohmann::json json = rtecliJSON(m_host, "tables list");

  for (auto &elem : json) {
    string table = elem["tbl_name"];
    m_state[table] = {};
    m_tables.push_back(table);

    updateTableState(table);
  }
}
