#include "tab.h"

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>

using namespace std;
using namespace ftxui;

Tab::Tab (const string name, const string host)
{
  m_tab_name = name;
  m_host = host;
}

Tab::~Tab () {}

string
Tab::getName ()
{
  return m_tab_name;
}
