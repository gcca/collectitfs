cmake_minimum_required(VERSION 3.31)

project(collectitfs VERSION 1.0.0 LANGUAGES CXX)

option(CITFS_ENABLE_OPT OFF)

find_package(Boost REQUIRED COMPONENTS program_options)

add_executable(collectitfs collectitfs.cc)
target_link_libraries(collectitfs PRIVATE Boost::program_options)

if (CITFS_ENABLE_OPT)
  message(STATUS "CTIFS: Enabling OPT")
  target_compile_options(collectitfs PRIVATE $<$<NOT:$<CONFIG:Debug>>:-Ofast -march=native -flto>)
  target_link_options(collectitfs PRIVATE $<$<NOT:$<CONFIG:Debug>>:-flto>)
else()
  message(STATUS "CTIFS: No OPT")
endif()

install(TARGETS collectitfs DESTINATION bin)
