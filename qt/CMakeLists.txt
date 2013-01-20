project(pddby-gui)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

find_package(Qt4 4.8 REQUIRED QtCore QtGui)
include(${QT_USE_FILE})

set(${PROJECT_NAME}_SOURCES
    main.cpp
    MainWindow.cpp
)

set(${PROJECT_NAME}_HEADERS
    MainWindow.h
)

qt4_wrap_ui(${PROJECT_NAME}_UI_SOURCES
    MainWindow.ui
)

qt4_add_resources(${PROJECT_NAME}_QRC_SOURCES
    PddByGui.qrc
)

add_executable(${PROJECT_NAME} MACOSX_BUNDLE
    ${${PROJECT_NAME}_SOURCES}
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_UI_SOURCES}
    ${${PROJECT_NAME}_QRC_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC ON)

target_link_libraries(${PROJECT_NAME} pddby ${QT_LIBRARIES})
