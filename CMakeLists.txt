cmake_minimum_required(VERSION 2.8)
project(homophonic-solver)

#set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Weffc++ -g -O3 -Ofast -flto -fwhole-program")
set(CMAKE_CXX_FLAGS "-std=c++11")
set(MAIN_SOURCES src/solver.cpp src/language_data.cpp src/message.cpp src/key.cpp src/main.cpp)
set(TEST_SOURCES src/test/tests.cpp)
set(MAIN_FLAGS "-std=c++11 -Wall -Wextra -Weffc++ -g -O3 -Ofast -flto -fwhole-program")
set(TEST_FLAGS "-std=c++11 -O2")

add_executable(homophonic-solver ${MAIN_SOURCES})
add_library(solver STATIC src/solver.cpp src/language_data.cpp src/message.cpp src/key.cpp src/main.cpp)
set_target_properties(homophonic-solver PROPERTIES COMPILE_FLAGS "${MAIN_FLAGS} -DEXECUTABLE")

add_executable(tests ${MAIN_SOURCES} ${TEST_SOURCES})
set_target_properties(tests PROPERTIES COMPILE_FLAGS "${TEST_FLAGS}")


install(TARGETS homophonic-solver RUNTIME DESTINATION bin)
install(TARGETS tests RUNTIME DESTINATION bin)
