# Copyright jdknight

cmake_minimum_required(VERSION 3.11)
project(w11asi)

set(BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(INC_DIR ${BASE_DIR}/src)
set(SRC_DIR ${BASE_DIR}/src/w11asi)
set(SCRIPTS_DIR ${BASE_DIR}/scripts)
include(${BASE_DIR}/support.cmake)

include(${BASE_DIR}/sources.cmake)
include_directories(${INC_DIR})

add_executable(w11asi WIN32 ${W11ASI_SRCS})
install(TARGETS w11asi RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES ${W11ASI_SCRIPTS} DESTINATION ${CMAKE_INSTALL_PREFIX})
