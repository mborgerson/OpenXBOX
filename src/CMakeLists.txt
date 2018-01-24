file(GLOB DIR_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

file(GLOB DIR_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

if (USE_CPU_UNICORN)
set(DIR_SOURCES ${DIR_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu_uc.cpp
    )
endif (USE_CPU_UNICORN)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/kernel")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/video")

set(SOURCES ${SOURCES}
    ${DIR_HEADERS}
    ${DIR_SOURCES}
    PARENT_SCOPE
    )
