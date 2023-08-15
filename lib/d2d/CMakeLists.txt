add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/collision")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/world")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/storage")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/video")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/motion")

set(SOURCE ${SOURCE} PARENT_SCOPE)
