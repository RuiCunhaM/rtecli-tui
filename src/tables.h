#ifndef TABLES_H
#define TABLES_H

#include <map>
#include <string>
#include <vector>

#include "components/tab.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/node.hpp>

using namespace std;

class Tables : public Tab {

public:
  Tables(const string name, const string host);
  ~Tables();

  void updateState();
  ftxui::Component render();
  bool handleEvent(ftxui::Event event);

private:
  int m_table_selected;
  vector<string> m_table_names;
  map<string, vector<vector<string>>> m_state;

  std::vector<std::vector<std::string>> repr();
  void initTables();
  mutex m_mutex;
};

#endif
