project(homophonic-solver)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Weffc++ -g -O0")

add_executable(homophonic-solver src/solver.cpp src/language_data.cpp src/message.cpp src/key.cpp src/main.cpp)

install(TARGETS homophonic-solver RUNTIME DESTINATION bin)