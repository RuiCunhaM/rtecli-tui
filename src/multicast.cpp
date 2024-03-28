#include "multicast.h"
#include "util.h"

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>

#include <nlohmann/json.hpp>

using namespace std;
using namespace ftxui;

MulticastGroups::MulticastGroups(const string host) {
  m_host = host;

  initMulticastGroups();
}

MulticastGroups::~MulticastGroups() {}

vector<vector<string>> MulticastGroups::getState() {
  vector<vector<string>> r;
  lock_guard<mutex> lock(m_mutex);
  r = m_state;
  return r;
}

void MulticastGroups::updateState() {
  vector<vector<string>> new_state;

  nlohmann::json json = rtecliJSON(m_host, "multicast list");
  for (auto &elem : json) {
    vector<string> ports;
    for (auto port : elem["ports"])
      ports.push_back(to_string(port));
    new_state.push_back(ports);
  }

  lock_guard<mutex> lock(m_mutex);
  m_state = new_state;
}

vector<string> MulticastGroups::getGroupsNumber() {
  vector<string> numbers;
  int i = 0;
  lock_guard<mutex> lock(m_mutex);
  for (auto e : m_state)
    numbers.push_back("mg" + to_string(i++));
  return numbers;
}

vector<vector<string>> MulticastGroups::getGroupState(int group_id) {
  vector<string> ports = m_state[group_id];
  vector<vector<string>> r;
  r.push_back({"ports", "dec", "hex"});
  for (string port : ports)
    r.push_back({"", port, unsigned2hexa(port)});
  return r;
}

void MulticastGroups::initMulticastGroups() { updateState(); }
