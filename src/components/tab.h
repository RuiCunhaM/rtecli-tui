#ifndef TAB_H
#define TAB_H

#include <string>
#include <vector>

#include <ftxui/component/component.hpp>
#include <ftxui/dom/node.hpp>

class Tab {
public:
  Tab(const std::string name, const std::string host);
  ~Tab();

  virtual void updateState() = 0;
  virtual ftxui::Component render() = 0;
  virtual bool handleEvent(ftxui::Event event) = 0;
  std::string getName();

protected:
  std::string m_tab_name;
  std::string m_host;
  virtual std::vector<std::vector<std::string>> repr() = 0;
};

#endif
