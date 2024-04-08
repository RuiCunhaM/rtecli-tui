#include "multicast.h"
#include "util.h"

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>

#include <nlohmann/json.hpp>

using namespace std;
using namespace ftxui;

MulticastGroups::MulticastGroups(const string name, const string host)
    : Tab(name, host) {
  initMulticastGroups();
}

MulticastGroups::~MulticastGroups() {}

void MulticastGroups::updateState() {
  m_state.clear();
  nlohmann::json json = rtecliJSON(m_host, "multicast list");
  for (auto &elem : json) {
    vector<string> ports;
    for (auto port : elem["ports"])
      ports.push_back(to_string(port));
    m_state.push_back(ports);
  }
}

vector<string> MulticastGroups::getGroupsNumber() {
  vector<string> numbers;
  int i = 0;
  for (auto e : m_state)
    numbers.push_back("mg" + to_string(i++));
  return numbers;
}

vector<vector<string>> MulticastGroups::repr() {
  vector<string> ports = m_state[m_group_selected];
  vector<vector<string>> r;
  r.push_back({"ports", "dec", "hex"});
  for (string port : ports)
    r.push_back({"", port, unsigned2hexa(port)});
  return r;
}

void MulticastGroups::initMulticastGroups() {
  updateState();
  int i = 0;
  for (auto e : m_state)
    m_group_names.push_back("mg" + to_string(i++));
  m_group_selected = 0;
}

Component MulticastGroups::render() {
  auto multicast_groups_drop = Dropdown(&m_group_names, &m_group_selected);

  auto multicast_groups_table = Renderer([this]() {
    Table t = Table(this->repr());
    t.SelectAll().Separator(LIGHT);
    styleTable(&t);
    return t.Render();
  });

  return Container::Vertical({
      multicast_groups_drop,
      multicast_groups_table,
  });
}

void MulticastGroups::handleEvent(Event event){};
