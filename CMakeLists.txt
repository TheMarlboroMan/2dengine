cmake_minimum_required(VERSION 3.5.1)

project(d2d)

option(BUILD_DEBUG "Build a debug release" "ON")
option(BUILD_FOR_RELEASE "Build for release" "OFF")


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(${BUILD_DEBUG})
	message("DEBUG BUILD")
	set(CMAKE_BUILD_TYPE Debug)
else()
	message("RELEASE BUILD")
	set(CMAKE_BUILD_TYPE Release)
endif()

if(${BUILD_FOR_RELEASE})
	message("Building with release paths")
else()
	message("Building with dev paths")
endif()

#-Werror
add_compile_options( -Wall -Wextra -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -Wno-deprecated -ansi -pedantic -fno-rtti)

include_directories("${CMAKE_CURRENT_LIST_DIR}/include")

add_subdirectory("${PROJECT_SOURCE_DIR}/lib")

#add the framework dependencies
#add_library(lm SHARED IMPORTED)
#add_library(tools SHARED IMPORTED)
#add_library(ldtools SHARED IMPORTED)
#add_library(dansdl2 SHARED IMPORTED)
#add_library(dfw SHARED IMPORTED)
#add_library(ascript SHARED IMPORTED)

#Virtual library so we don't have to build again and again for all tests...
option(BUILD_TESTS "Build development tools tests" "ON")

#add_executable(plataformer src/main.cpp)
add_library(d2d_static STATIC ${SOURCE})
set_target_properties(d2d_static PROPERTIES OUTPUT_NAME d2d)

add_library(d2d_shared SHARED ${SOURCE})
set_target_properties(d2d_shared PROPERTIES OUTPUT_NAME d2d)

IF(WIN32)
	include_directories("include")
	include_directories("${CMAKE_CURRENT_LIST_DIR}/../log/include/")
	include_directories("${CMAKE_CURRENT_LIST_DIR}/../libdansdl2/include/")
	include_directories("${CMAKE_CURRENT_LIST_DIR}/../libdansdl2-tools/include/")
	include_directories("${CMAKE_CURRENT_LIST_DIR}/../tools/include/")
	include_directories("${CMAKE_CURRENT_LIST_DIR}/../dfw/include/")
	include_directories("${CMAKE_CURRENT_LIST_DIR}/../ascript/include/")
	include_directories("${CMAKE_CURRENT_LIST_DIR}/../rapidjson/include/")

	set_target_properties(lm PROPERTIES IMPORTED_IMPLIB "C:/Program Files (x86)/lm/lib/liblm.dll")
	set_target_properties(dansdl2 PROPERTIES IMPORTED_IMPLIB "C:/Program Files (x86)/libdansdl2/lib/libdansdl2.dll")
	set_target_properties(ldtools PROPERTIES IMPORTED_IMPLIB "C:/Program Files (x86)/libldtools/lib/libldtools.dll")
	set_target_properties(tools PROPERTIES IMPORTED_IMPLIB "C:/Program Files (x86)/tools/lib/libtools.dll")
	set_target_properties(dfw PROPERTIES IMPORTED_IMPLIB "C:/Program Files (x86)/dfw/lib/libdfw.dll")
	set_target_properties(ascript PROPERTIES IMPORTED_IMPLIB "C:/Program Files (x86)/dfw/lib/ascript.dll")

# 	target_link_libraries(plataformer applib dfw tools dansdl2 ldtools tools lm SDL2 SDL2_ttf SDL2_mixer SDL2_image freeglut glew32 glu32 opengl32 stdc++fs)
#	target_link_options(plataformer PRIVATE -lmingw32 -lSDL2main -lSDL2 -mwindows)

ELSE()

#	if(${BUILD_FOR_RELEASE})
#		add_definitions(-DBUILD_FOR_RELEASE)
#		set_target_properties(lm PROPERTIES IMPORTED_LOCATION /usr/local/lib/liblm.a)
#		set_target_properties(tools PROPERTIES IMPORTED_LOCATION /usr/local/lib/libtools.a)
#		set_target_properties(dansdl2 PROPERTIES IMPORTED_LOCATION /usr/local/lib/libdansdl2.a)
#		set_target_properties(ldtools PROPERTIES IMPORTED_LOCATION /usr/local/lib/libldtools.a)
#		set_target_properties(dfw PROPERTIES IMPORTED_LOCATION /usr/local/lib/libdfw.a)
#		set_target_properties(ascript PROPERTIES IMPORTED_LOCATION /usr/local/lib/libascript.a)
#	else()
#		set_target_properties(lm PROPERTIES IMPORTED_LOCATION /usr/local/lib/liblm.so)
#		set_target_properties(tools PROPERTIES IMPORTED_LOCATION /usr/local/lib/libtools.so)
#		set_target_properties(dansdl2 PROPERTIES IMPORTED_LOCATION /usr/local/lib/libdansdl2.so)
#		set_target_properties(ldtools PROPERTIES IMPORTED_LOCATION /usr/local/lib/libldtools.so)
#		set_target_properties(dfw PROPERTIES IMPORTED_LOCATION /usr/local/lib/libdfw.so)
#		set_target_properties(ascript PROPERTIES IMPORTED_LOCATION /usr/local/lib/libascript.so)
#	endif()

#	target_link_libraries(plataformer applib ascript dfw tools dansdl2 ldtools lm SDL2 SDL2_ttf SDL2_mixer SDL2_image GL stdc++fs pthread)

	if(${BUILD_FOR_RELEASE})

		#set(CPACK_GENERATOR "DEB")
		#set(CPACK_DEBIAN_PACKAGE_MAINTAINER "The Marlboro Man")
		#set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsdl2-2.0-0, libsdl2-image-2.0-0, libsdl2-mixer-2.0-0, libsdl2-ttf-2.0-0, libstdc++6")
		#include(CPack)

		#install(TARGETS plataformer RUNTIME DESTINATION /usr/local/bin)

		#install(
		#	DIRECTORY ${CMAKE_SOURCE_DIR}/resources/data
		#	DESTINATION /usr/share/plataformer/
		#	USE_SOURCE_PERMISSIONS
		#	COMPONENT plataformer
		#)

		#install(
		#	DIRECTORY ${CMAKE_SOURCE_DIR}/resources/runtime
		#	DESTINATION "$ENV{HOME}/.plataformer"
		#	USE_SOURCE_PERMISSIONS
		#	COMPONENT plataformer
		#)

		#install(
		#	DIRECTORY ${CMAKE_SOURCE_DIR}/resources/savegame
		#	DESTINATION "$ENV{HOME}/.plataformer"
		#	USE_SOURCE_PERMISSIONS
		#	COMPONENT plataformer
		#)

	endif()

	if(${BUILD_TESTS})

		#add_executable(background_builder src/tests/background_builder.cpp)
		#target_link_libraries(background_builder applib ascript dfw ldtools tools dansdl2 lm SDL2 SDL2_ttf SDL2_mixer SDL2_image GL stdc++fs)

		#add_executable(savegame_reader src/tests/savegame_reader.cpp)
		#target_link_libraries(savegame_reader applib ascript dfw ldtools tools dansdl2 lm SDL2 SDL2_ttf SDL2_mixer SDL2_image GL stdc++fs)
	endif()
ENDIF()

#Configure the executable, we want it into the "build directory" and we want all resources copied
#to the build directory.
#add_custom_target(setup_files ALL)
#add_custom_command(TARGET setup_files POST_BUILD COMMAND cp -r ../resources/* ./)
#add_custom_command(TARGET setup_files POST_BUILD COMMAND cp ../integration/background-tiles.txt ../resources/data/sheets/background.txt)
#add_custom_command(TARGET setup_files POST_BUILD COMMAND cp ../integration/background-tiles.png ../resources/data/bitmap/background.png)

