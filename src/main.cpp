#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <thread>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/loop.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "multicast.h"
#include "ports.h"
#include "registers.h"
#include "system-counters.h"
#include "tables.h"
#include "util.h"

#define INTERVAL 1s

#define TAB_SYS_COUNTERS 0
#define TAB_REGISTERS 1
#define TAB_TABLES 2
#define TAB_MULTICAST_GROUPS 3
#define TAB_PORTS_GROUPS 4

using namespace ftxui;
using namespace std::chrono_literals;
using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cout << "A host to connect is required" << endl;
    exit(1);
  }

  // #####################
  // ## System Counters ##
  // #####################

  SystemCounters sysCnt = SystemCounters(argv[1]);

  auto sysCnt_table = Renderer([&sysCnt]() {
    Table t = Table(sysCnt.getState());
    styleTable(&t);
    return t.Render();
  });

  auto sysCntContainer = Container::Horizontal({
      sysCnt_table,
      Button("Clear System Counters",
             [&sysCnt] { sysCnt.clearSysCounters(); }) |
          size(HEIGHT, EQUAL, 1),
  });

  // #####################
  // ##### Registers #####
  // #####################

  Registers reg = Registers(argv[1]);

  auto reg_table = Renderer([&reg]() {
    Table t = Table(reg.getState());
    styleTable(&t);
    return t.Render();
  });

  auto regContainer = Container::Horizontal({
      reg_table,
      Button("Clear Registers", [&reg] { reg.clearRegisters(); }) |
          size(HEIGHT, EQUAL, 1),
  });

  // #####################
  // ###### Tables #######
  // #####################

  Tables tables = Tables(argv[1]);

  int table_selected = 0;
  vector<string> table_entries = tables.getTables();

  auto table_drop = Dropdown(&table_entries, &table_selected);
  auto table_render = Renderer([&tables, &table_selected, &table_entries]() {
    Table t(tables.getTableState(table_entries.at(table_selected)));
    t.SelectAll().Separator(LIGHT);
    styleTable(&t);
    return t.Render();
  });
  auto tables_render = Container::Vertical({
      table_drop,
      table_render,
  });

  // ######################
  // ##### Multicast ######
  // ######################

  MulticastGroups multicast_groups = MulticastGroups(argv[1]);
  int group_selected = 0;
  vector<string> groups = multicast_groups.getGroupsNumber();

  auto multicast_groups_drop = Dropdown(&groups, &group_selected);

  auto multicast_groups_table =
      Renderer([&multicast_groups, &group_selected]() {
        Table t = Table(multicast_groups.getGroupState(group_selected));
        t.SelectAll().Separator(LIGHT);
        styleTable(&t);
        return t.Render();
      });

  auto multicast_groups_render = Container::Vertical({
      multicast_groups_drop,
      multicast_groups_table,
  });

  // #####################
  // ####### Ports #######
  // #####################

  Ports ports = Ports(argv[1]);

  auto ports_table = Renderer([&ports]() {
    Table t = Table(ports.getState());
    styleTable(&t);
    return t.Render();
  });

  // #####################
  // ###### Layout #######
  // #####################

  auto screen = ScreenInteractive::Fullscreen();

  atomic<bool> refresh_ui_continue = true;
  int tab_selected = TAB_SYS_COUNTERS;

  vector<string> tab_values{
      "System Counters", "Registers", "Tables", "Multicast", "Ports",
  };
  auto tab_toggle = Toggle(&tab_values, &tab_selected);

  auto tab_container = Container::Tab(
      {
          sysCntContainer,
          regContainer,
          tables_render,
          multicast_groups_render,
          ports_table,
      },
      &tab_selected);

  auto container = Container::Vertical({
      tab_toggle,
      tab_container,
  });

  auto render = Renderer(container, [&] {
    return vbox({
        tab_toggle->Render(),
        separator(),
        tab_container->Render(),
        filler(),
        text("hjkl - navigate | q - quit") | inverted | center,
    });
  });

  thread refresh_ui([&] {
    while (refresh_ui_continue) {
      this_thread::sleep_for(INTERVAL);

      // NOTE: Having the main thread handle state updates prevents possible
      // concurrency issues, however, if a state update hangs, the entire
      // program becomes unresponsive. We should fix this.
      screen.Post([&] {
        switch (tab_selected) {
        case TAB_SYS_COUNTERS:
          sysCnt.updateState();
          break;
        case TAB_REGISTERS:
          reg.updateState();
          break;
        case TAB_TABLES:
          tables.updateTableState(table_entries.at(table_selected));
          break;
        case TAB_MULTICAST_GROUPS:
          multicast_groups.updateState();
          break;
        case TAB_PORTS_GROUPS:
          ports.updateState();
          break;
        default:
          // Do nothing
          break;
        }
      });

      // Redraw frame
      screen.Post(Event::Custom);
    }
  });

  screen.Loop(render | CatchEvent([&](Event event) {
                if (event == Event::Character("q")) {
                  screen.ExitLoopClosure()();
                  return true;
                }
                return false;
              }));

  refresh_ui_continue = false;
  refresh_ui.join();

  return 0;
}
