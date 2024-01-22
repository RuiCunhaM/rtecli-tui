#include "util.h"

#include <cstdio>
#include <format>
#include <memory>
#include <string>
#include <unistd.h>

using namespace std;

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
  string result = rtecli(host, args);

  if (result != "ERROR")
    replace(result.begin(), result.end(), '\'', '"');

  return nlohmann::json::parse(result);
}

string hexa2integer(const string hexa) {
  int tmp = stoul(hexa, nullptr, 16);
  tmp = ~tmp;
  return to_string(-(tmp + 1));
}

string hexa2unsigned(const string hexa) {
  return to_string(stoul(hexa, nullptr, 16));
}
