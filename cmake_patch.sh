#!/bin/bash
echo "add_library (sign_service SHARED ../sign_service.hpp ../sign_service.cpp)
target_link_libraries (sign_service node)">>raiblocks/CMakeLists.txt
