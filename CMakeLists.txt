cmake_minimum_required(VERSION 3.5)

project(libdomain-qt-sample VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(FindLdap)
include(FindLibdomain)

find_package(Ldap REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(Libverto REQUIRED IMPORTED_TARGET libverto)
pkg_check_modules(Glib REQUIRED IMPORTED_TARGET glib-2.0)
pkg_check_modules(Talloc REQUIRED IMPORTED_TARGET talloc)
pkg_check_modules(Libconfig REQUIRED IMPORTED_TARGET libconfig)


set(PROJECT_SOURCES
        attributesmodel.h
        attributesmodel.cpp
        connectiondialog.h
        connectiondialog.cpp
        connectionsettings.h
        connectionsettings.cpp
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

add_executable(libdomain-qt-test
    ${PROJECT_SOURCES}
)

target_link_libraries(libdomain-qt-test PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(libdomain-qt-test PRIVATE PkgConfig::Libverto PkgConfig::Glib PkgConfig::Talloc PkgConfig::Libconfig)
target_link_libraries(libdomain-qt-test PRIVATE Ldap::Ldap)
target_link_libraries(libdomain-qt-test PRIVATE Libdomain::Libdomain)

set_target_properties(libdomain-qt-test PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.basealt.ru
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS libdomain-qt-test
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
