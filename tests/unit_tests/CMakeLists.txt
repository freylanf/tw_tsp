include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Unit_Tests_run FptSolverTest.cpp GraphTest.cpp ../../src/Graph.cpp ../../src/FptSolver.cpp)

include_directories(test_data)

target_link_libraries(Unit_Tests_run gtest gtest_main)
target_link_libraries(Unit_Tests_run ${CMAKE_PROJECT_NAME}_lib)