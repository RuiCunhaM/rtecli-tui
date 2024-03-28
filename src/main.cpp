#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <thread>

#include "components/tab.h"
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
#include <memory>

#define INTERVAL 10s

using namespace ftxui;
using namespace std::chrono_literals;
using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cout << "A host to connect is required" << endl;
    exit(1);
  }

  // #####################
  // ###### Layout #######
  // #####################

  vector<unique_ptr<Tab>> tabs;
  tabs.push_back(make_unique<SystemCounters>("System Counters", argv[1]));
  tabs.push_back(make_unique<Registers>("Registers", argv[1]));
  tabs.push_back(make_unique<Tables>("Tables", argv[1]));
  tabs.push_back(make_unique<MulticastGroups>("Multicast", argv[1]));
  tabs.push_back(make_unique<Ports>("Ports", argv[1], 10));

  auto screen = ScreenInteractive::Fullscreen();

  atomic<bool> refresh_ui_continue = true;
  int tab_selected = 0;

  vector<string> tab_names;
  for (const auto &tab_ptr : tabs)
    tab_names.push_back(tab_ptr->getName());

  auto tab_toggle = Toggle(&tab_names, &tab_selected);

  vector<Component> tab_rendered;
  for (const auto &tab_ptr : tabs)
    tab_rendered.push_back(tab_ptr->render());

  auto tab_container = Container::Tab(tab_rendered, &tab_selected);

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
        auto &tab_ptr = tabs[tab_selected];
        tab_ptr->updateState();
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
                auto &tab_ptr = tabs[tab_selected];
                tab_ptr->handleEvent(event);
                return false;
              }));

  refresh_ui_continue = false;
  refresh_ui.join();

  return 0;
}
