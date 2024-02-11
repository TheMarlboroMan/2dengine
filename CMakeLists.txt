cmake_minimum_required(VERSION 3.5.1)
project(d2d)

option(BUILD_DEBUG "Build a debug release" ON)
option(BUILD_SHARED "Build a shared library" ON)
option(BUILD_STATIC "Build a static library" OFF)
option(BUILD_EXAMPLE "Build example" "ON")

#library version
set(MAJOR_VERSION 0)
set(MINOR_VERSION 0)
set(PATCH_VERSION 0)

if(${BUILD_DEBUG})

	set(CMAKE_BUILD_TYPE Debug)
else()

	set(CMAKE_BUILD_TYPE Release)
endif()

if(${CMAKE_VERSION} VERSION_LESS "3.22.0")

	add_definitions(-DMAJOR_VERSION=${MAJOR_VERSION})
	add_definitions(-DMINOR_VERSION=${MINOR_VERSION})
	add_definitions(-DPATCH_VERSION=${PATCH_VERSION})
	add_definitions(-DRELEASE_VERSION=\"${RELEASE_VERSION}\")
else()

	add_compile_definitions(MAJOR_VERSION=${MAJOR_VERSION})
	add_compile_definitions(MINOR_VERSION=${MINOR_VERSION})
	add_compile_definitions(PATCH_VERSION=${PATCH_VERSION})
	add_compile_definitions(RELEASE_VERSION=\"${RELEASE_VERSION}\")
endif()


#-Werror
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options( -Wall -Wextra -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -Wno-deprecated -ansi -pedantic -fno-rtti)

include_directories("${CMAKE_CURRENT_LIST_DIR}/include")

#Define the library itself.
set(SOURCE "")
add_subdirectory("${PROJECT_SOURCE_DIR}/lib")

#library type and filenames.
if(${BUILD_DEBUG})

	set(CMAKE_BUILD_TYPE DEBUG)
	set(LIB_FILENAME "d2d_debug")
else()

	set(CMAKE_BUILD_TYPE RELEASE)
	set(LIB_FILENAME "d2d")
endif()

if(${BUILD_STATIC}) 

	add_library(d2d_static STATIC ${SOURCE})
	set_target_properties(d2d_static PROPERTIES OUTPUT_NAME ${LIB_FILENAME})
	target_compile_definitions(d2d_static PUBLIC "-DLIB_VERSION=\"static\"")
	install(TARGETS d2d_static DESTINATION lib)

	if(${BUILD_DEBUG})

		target_compile_definitions(d2d_static PUBLIC "-DLIBDANSDL2_DEBUG")
	endif()

	message("will build ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}-${RELEASE_VERSION}-static")
endif()

if(${BUILD_SHARED})

	add_library(d2d_shared SHARED ${SOURCE})
	set_target_properties(d2d_shared PROPERTIES OUTPUT_NAME ${LIB_FILENAME})
	target_compile_definitions(d2d_shared PUBLIC "-DLIB_VERSION=\"shared\"")
	install(TARGETS d2d_shared DESTINATION lib)

	if(${BUILD_DEBUG})

		target_compile_definitions(d2d_shared PUBLIC "-DLIBDANSDL2_DEBUG")
	endif()

	message("will build ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}-${RELEASE_VERSION}-shared")
endif()

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

	if(${BUILD_DEBUG})
		set_target_properties(lm PROPERTIES IMPORTED_LOCATION /usr/local/lib/liblm_debug.so)
		set_target_properties(tools PROPERTIES IMPORTED_LOCATION /usr/local/lib/libtools_debug.so)
		set_target_properties(dansdl2 PROPERTIES IMPORTED_LOCATION /usr/local/lib/libdansdl2_debug.so)
		set_target_properties(ldtools PROPERTIES IMPORTED_LOCATION /usr/local/lib/libldtools_debug.so)
		set_target_properties(dfw PROPERTIES IMPORTED_LOCATION /usr/local/lib/libdfw_debug.so)
	else()
		set_target_properties(lm PROPERTIES IMPORTED_LOCATION /usr/local/lib/liblm.so)
		set_target_properties(tools PROPERTIES IMPORTED_LOCATION /usr/local/lib/libtools.so)
		set_target_properties(dansdl2 PROPERTIES IMPORTED_LOCATION /usr/local/lib/libdansdl2.so)
		set_target_properties(ldtools PROPERTIES IMPORTED_LOCATION /usr/local/lib/libldtools.so)
		set_target_properties(dfw PROPERTIES IMPORTED_LOCATION /usr/local/lib/libdfw.so)
	endif()

	add_executable(example-app example/src/main.cpp)

	if(${BUILD_DEBUG})

		target_compile_definitions(example-app PUBLIC "-DLIBDANSDL2_DEBUG")
	endif()

	target_link_libraries(example-app exlib d2d_static dfw tools dansdl2 ldtools lm SDL2 SDL2_ttf SDL2_mixer SDL2_image GL stdc++fs pthread)

	add_custom_target(setup_files ALL)
	add_custom_command(TARGET setup_files POST_BUILD COMMAND cp -r ../example/resources .)
endif()
