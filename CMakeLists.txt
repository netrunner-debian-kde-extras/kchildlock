project(kchildlock)

cmake_minimum_required(VERSION 2.6)

find_package( KDE4 REQUIRED )
include_directories( ${KDE4_INCLUDES} )

include_directories (
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/kcmodule  ${CMAKE_CURRENT_BINARY_DIR}/daemon ${CMAKE_CURRENT_BINARY_DIR}/common
    ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/kcmodule  ${CMAKE_CURRENT_BINARY_DIR}/common
)

add_subdirectory(common)
add_subdirectory(kcmodule)
add_subdirectory(daemon)
add_subdirectory(doc)
add_subdirectory(po)
add_subdirectory(pics)
