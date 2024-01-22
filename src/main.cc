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

#define INTERVAL 1s

#define TAB_SYS_COUNTERS 0
#define TAB_REGISTERS 1

using namespace ftxui;
using namespace std::chrono_literals;
using namespace std;

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

int main(int argc, char *argv[]) {

  if (argc < 2) {
    cout << "A host to connect is required" << endl;
    exit(1);
  }

  Registers reg = Registers(argv[1]);
  SystemCounters sysCnt = SystemCounters(argv[1]);

  auto screen = ScreenInteractive::Fullscreen();

  atomic<bool> refresh_ui_continue = true;
  int tab_selected = TAB_SYS_COUNTERS;

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
        default:
          // Do nothing
          break;
        }
      });

      // Redraw frame
      screen.Post(Event::Custom);
    }
  });

  vector<string> tab_values{
      "System Counters",
      "Registers",
  };
  auto tab_toggle = Toggle(&tab_values, &tab_selected);

  auto sysCnt_render = Renderer([&sysCnt]() {
    Table t = Table(sysCnt.getState());
    styleTable(&t);
    return t.Render();
  });

  auto reg_render = Renderer([&reg]() {
    Table t = Table(reg.getState());
    styleTable(&t);
    return t.Render();
  });

  auto tab_container = Container::Tab(
      {
          sysCnt_render,
          reg_render,
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
        text("tab/hl - cycle through tabs | q - quit") | inverted | center,
    });
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
