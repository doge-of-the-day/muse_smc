add_definitions(-W -Wall -Wno-unused-parameter -fno-strict-aliasing -Wno-unused-function -Wno-deprecated-register)
add_definitions(-march=native -ffast-math)

if(NOT ${CMAKE_BUILD_TYPE} STREQUAL Debug)
add_definitions("-Ofast")
add_definitions(-g -rdynamic)
endif()
