project(pddby-test)

add_definitions(-DBOOST_TEST_DYN_LINK)

set(${PROJECT_NAME}_SOURCES
    FilterTestSuite.cpp
    main.cpp
    OakTestSuite.cpp
)

include_directories(.. ../lib)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})
target_link_libraries(${PROJECT_NAME}
    pddby
)
