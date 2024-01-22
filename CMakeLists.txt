cmake_minimum_required (VERSION 3.11)

include(FetchContent)
 
# --- Fetch FTXUI --------------------------------------------------------------
 
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v5.0.0
)
 
FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
 
# --- Fetch JSON --------------------------------------------------------------

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

# ------------------------------------------------------------------------------

set (CMAKE_CXX_STANDARD 20)

project(rtecli-tui
  LANGUAGES CXX
  VERSION 1.0.0
)

set(SOURCES_DIR src)
file(GLOB_RECURSE SOURCES ${SOURCES_DIR}/*.cc)
add_executable(rtecli-tui ${SOURCES})

target_include_directories(rtecli-tui PRIVATE src)
 
target_link_libraries(rtecli-tui
  PRIVATE ftxui::component
  PRIVATE nlohmann_json::nlohmann_json
)
