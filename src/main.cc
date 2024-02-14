#include <thread>

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/loop.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>

#include "registers.h"
#include "system-counters.h"
#include "tables.h"
#include "util.h"

#define INTERVAL 1s

#define TAB_SYS_COUNTERS 0
#define TAB_REGISTERS 1
#define TAB_TABLES 2

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

  auto sysCnt_render = Renderer([&sysCnt]() {
    Table t = Table(sysCnt.getState());
    styleTable(&t);
    return t.Render();
  });

  // #####################
  // ##### Registers #####
  // #####################

  Registers reg = Registers(argv[1]);

  auto reg_render = Renderer([&reg]() {
    Table t = Table(reg.getState());
    styleTable(&t);
    return t.Render();
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

  // #####################
  // ###### Layout #######
  // #####################

  auto screen = ScreenInteractive::Fullscreen();

  atomic<bool> refresh_ui_continue = true;
  int tab_selected = TAB_SYS_COUNTERS;

  vector<string> tab_values{
      "System Counters",
      "Registers",
      "Tables",
  };
  auto tab_toggle = Toggle(&tab_values, &tab_selected);

  auto tab_container = Container::Tab(
      {
          sysCnt_render,
          reg_render,
          tables_render,
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
