#include "util.h"

#include <cstdio>
#include <format>
#include <memory>
#include <string>
#include <unistd.h>

using namespace std;
using namespace ftxui;

#define BUFFSIZE 1024

char buffer[BUFFSIZE];

string exec(const string cmd) {
  shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);

  if (!pipe)
    return "ERROR";

  string result = "";
  while (!feof(pipe.get())) {
    if (fgets(buffer, BUFFSIZE, pipe.get()) != NULL)
      result += buffer;
  }

  return result;
}

string rtecli(const string host, const string args) {
  return exec(format("/opt/netronome/p4/bin/rtecli -r {} {}", host, args));
}

nlohmann::json rtecliJSON(const string host, const string args) {
  string result = rtecli(host, format("--json {}", args));

  result.erase(remove(result.begin(), result.end(), '\\'), result.end());

  while (result.find("\"{") != string::npos)
    result.replace(result.find("\"{"), 2, "{");

  while (result.find("}\"") != string::npos)
    result.replace(result.find("}\""), 2, "}");

  return nlohmann::json::parse(result);
}

string unsigned2hexa(const string input) {
    // Convert string to unsigned long long
    stringstream ss(input);
    unsigned long long value;
    ss >> value;

    // Convert unsigned long long to hex string
    stringstream hexStream;
    hexStream << hex << value;
    return "0x"+hexStream.str();
}

string hexa2integer(const string hexa) {
  int tmp = stoul(hexa, nullptr, 16);
  tmp = ~tmp;
  return to_string(-(tmp + 1));
}

string hexa2unsigned(const string hexa) {
  return to_string(stoul(hexa, nullptr, 16));
}

void styleTable(Table *t) {
  t->SelectAll().Border(LIGHT);
  t->SelectColumn(0).Border(LIGHT);

  t->SelectRow(0).Decorate(bold);
  t->SelectRow(0).SeparatorVertical(LIGHT);
  t->SelectRow(0).Border(DOUBLE);

  auto content = t->SelectRows(1, -1);
  content.DecorateCellsAlternateRow(color(Color::Blue), 3, 0);
  content.DecorateCellsAlternateRow(color(Color::Green), 3, 1);
  content.DecorateCellsAlternateRow(color(Color::White), 3, 2);
}
