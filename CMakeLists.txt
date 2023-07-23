cmake_minimum_required(VERSION 3.5.1)

project(d2d)

option(BUILD_DEBUG "Build a debug release" "ON")
option(BUILD_TESTS "Build development tools tests" "OFF")
option(BUILD_EXAMPLE "Build example" "ON")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(${BUILD_DEBUG})
	message("Debug build")
	set(CMAKE_BUILD_TYPE Debug)
else()
	message("Release build")
	set(CMAKE_BUILD_TYPE Release)
endif()

#-Werror
add_compile_options( -Wall -Wextra -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -Wno-deprecated -ansi -pedantic -fno-rtti)
include_directories("${CMAKE_CURRENT_LIST_DIR}/include")

#Define the library itself.
set(SOURCE "")
add_subdirectory("${PROJECT_SOURCE_DIR}/lib")

add_library(d2d_static STATIC ${SOURCE})
set_target_properties(d2d_static PROPERTIES OUTPUT_NAME d2d)

add_library(d2d_shared SHARED ${SOURCE})
set_target_properties(d2d_shared PROPERTIES OUTPUT_NAME d2d)

if(${BUILD_EXAMPLE})

	message("Will build example")

	include_directories("${CMAKE_CURRENT_LIST_DIR}/example/include")
	set(EXAMPLE_SOURCE "")
	add_subdirectory("${PROJECT_SOURCE_DIR}/example/src")
	add_library(exlib SHARED ${EXAMPLE_SOURCE})
	set_target_properties(exlib PROPERTIES OUTPUT_NAME exlib)

	#add the framework dependencies
	add_library(lm SHARED IMPORTED)
	add_library(tools SHARED IMPORTED)
	add_library(ldtools SHARED IMPORTED)
	add_library(dansdl2 SHARED IMPORTED)
	add_library(dfw SHARED IMPORTED)
	set_target_properties(lm PROPERTIES IMPORTED_LOCATION /usr/local/lib/liblm.so)
	set_target_properties(tools PROPERTIES IMPORTED_LOCATION /usr/local/lib/libtools.so)
	set_target_properties(dansdl2 PROPERTIES IMPORTED_LOCATION /usr/local/lib/libdansdl2.so)
	set_target_properties(ldtools PROPERTIES IMPORTED_LOCATION /usr/local/lib/libldtools.so)
	set_target_properties(dfw PROPERTIES IMPORTED_LOCATION /usr/local/lib/libdfw.so)

	add_executable(example-app example/src/main.cpp)
	target_link_libraries(example-app exlib d2d_static dfw tools dansdl2 ldtools lm SDL2 SDL2_ttf SDL2_mixer SDL2_image GL stdc++fs pthread)
endif()

if(${BUILD_TESTS})

	#add_executable(background_builder src/tests/background_builder.cpp)
	#target_link_libraries(background_builder applib ascript dfw ldtools tools dansdl2 lm SDL2 SDL2_ttf SDL2_mixer SDL2_image GL stdc++fs)

	#add_executable(savegame_reader src/tests/savegame_reader.cpp)
	#target_link_libraries(savegame_reader applib ascript dfw ldtools tools dansdl2 lm SDL2 SDL2_ttf SDL2_mixer SDL2_image GL stdc++fs)
endif()

#Configure the executable, we want it into the "build directory" and we want all resources copied
#to the build directory.
#add_custom_target(setup_files ALL)
#add_custom_command(TARGET setup_files POST_BUILD COMMAND cp -r ../resources/* ./)
#add_custom_command(TARGET setup_files POST_BUILD COMMAND cp ../integration/background-tiles.txt ../resources/data/sheets/background.txt)
#add_custom_command(TARGET setup_files POST_BUILD COMMAND cp ../integration/background-tiles.png ../resources/data/bitmap/background.png)

