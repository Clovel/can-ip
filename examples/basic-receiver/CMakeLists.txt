# 
#                     Copyright (C) 2020 Clovis Durand
# 
# -----------------------------------------------------------------------------

# Definitions ---------------------------------------------
add_definitions(-DEXAMPLE_BASIC_RECEIVER)

# Requirements --------------------------------------------

# Header files --------------------------------------------
file(GLOB_RECURSE PUBLIC_HEADERS 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../../*.hpp
)

set(HEADERS
    ${PUBLIC_HEADERS}
)

# Source files --------------------------------------------
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
)

# Target definition ---------------------------------------
add_executable(${CMAKE_PROJECT_NAME}-basic-receiver
    ${SOURCES}
)
add_dependencies(${CMAKE_PROJECT_NAME}-basic-receiver
    ${CMAKE_PROJECT_NAME}
)
target_link_libraries(${CMAKE_PROJECT_NAME}-basic-receiver
    ${CMAKE_PROJECT_NAME}
)
