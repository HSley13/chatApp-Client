cmake_minimum_required(VERSION 3.29.3)
project(client_app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)

find_package(Qt6 6.7.0 COMPONENTS Core Widgets Multimedia WebSockets)
qt_standard_project_setup()

qt_add_executable(client
                    client_main.cpp
                    client_main_window.cpp
                    client_manager.cpp
                    client_chat_window.cpp
                    chat_protocol.cpp
                    chat_line.cpp
                )

target_link_libraries(client PRIVATE 
                                    Qt6::Core 
                                    Qt6::Widgets 
                                    Qt6::Multimedia 
                                    Qt6::WebSockets)

qt_add_resources(client "client_images"
                    PREFIX "/images"
                    FILES send_icon.png file_icon.webp record_icon.png style.css
                )

set(MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)

set_target_properties(client PROPERTIES MACOSX_BUNDLE ON  MACOSX_BUNDLE_INFO_PLIST ${MACOSX_BUNDLE_INFO_PLIST})