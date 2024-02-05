#ifndef UTIL_H
#define UTIL_H

#include <string>

#include <ftxui/dom/table.hpp>
#include <nlohmann/json.hpp>

std::string exec(const std::string cmd);
std::string rtecli(const std::string host, const std::string args);
nlohmann::json rtecliJSON(const std::string host, const std::string args);

std::string hexa2integer(const std::string hexa);
std::string hexa2unsigned(const std::string hexa);

void styleTable(ftxui::Table *t);

#endif
