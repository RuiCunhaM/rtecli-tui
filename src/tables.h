#ifndef TABLES_H
#define TABLES_H

#include <map>
#include <string>
#include <vector>

#include <ftxui/dom/node.hpp>

using namespace std;

class Tables {

public:
  Tables(const string host);
  ~Tables();

  vector<string> getTables();
  void updateTableState(const string tableName);
  vector<vector<string>> getTableState(const string tableName);

private:
  string m_host;
  map<string, vector<vector<string>>> m_state;

  void initTables();
};

#endif
