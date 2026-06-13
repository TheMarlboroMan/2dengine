cmake_minimum_required(VERSION 3.5.1)
project(d2d)

option(BUILD_DEBUG "Build a debug release" ON)
option(BUILD_SHARED "Build a shared library" ON)
option(BUILD_STATIC "Build a static library" ON)
option(BUILD_EXAMPLES "Build example" ON)
option(BUILD_TESTS "Build test code" ON)
option(BUILD_EXAMPLE_APPIMAGE "Build example as appimage" OFF)

#library version
set(MAJOR_VERSION 0)
set(MINOR_VERSION 1)
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


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#-Werror => turn all warnings into errors!
#-Wfatal-errors => abort as soon as an error appears.
add_compile_options( -Wall -Wextra -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -Wno-deprecated -pedantic -fno-rtti)

include_directories("${CMAKE_CURRENT_LIST_DIR}/include")

#Define the library itself.
set(SOURCE "")
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/d2d/collision")
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/d2d/storage")
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/d2d/video")
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/d2d/audio")
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/d2d/motion")
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/d2d/components")

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

		target_compile_definitions(d2d_static PUBLIC "-DIS_DEBUG_BUILD")
	endif()

	message("will build ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}-${RELEASE_VERSION}-static")
endif()

if(${BUILD_SHARED})

	add_library(d2d_shared SHARED ${SOURCE})
	set_target_properties(d2d_shared PROPERTIES OUTPUT_NAME ${LIB_FILENAME})
	target_compile_definitions(d2d_shared PUBLIC "-DLIB_VERSION=\"shared\"")
	install(TARGETS d2d_shared DESTINATION lib)

	if(${BUILD_DEBUG})

		target_compile_definitions(d2d_shared PUBLIC "-DIS_DEBUG_BUILD")
	endif()

	message("will build ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}-${RELEASE_VERSION}-shared")
endif()

#Setup a lot of dependencies, we may not use them all unless we do a full build.

add_library(lm SHARED IMPORTED)
add_library(tools SHARED IMPORTED)
add_library(env SHARED IMPORTED)
add_library(console SHARED IMPORTED)
add_library(dansdl2 SHARED IMPORTED)
add_library(ldtools SHARED IMPORTED)
add_library(dfw SHARED IMPORTED)
add_library(appconsole SHARED IMPORTED)

set(IMPORTED_LIB_POSTFIX "")
if(${BUILD_DEBUG})

	set(IMPORTED_LIB_POSTFIX "_debug")
endif()

if(WIN32) 

	include_directories(../libdansdl2/include)
	include_directories(../ldtools/include)
	include_directories(../log/include)
	include_directories(../dfw/include)
	include_directories(../tools/include)
	include_directories(../appconsole/include)
	include_directories(../console/include)
	include_directories(../appenv/include)

	if(${BUILD_SHARED})

		target_compile_definitions(d2d_shared PUBLIC -DWINBUILD)

		#in windows we cannot allow unresolved symbols...
		set_target_properties(lm PROPERTIES 
			IMPORTED_IMPLIB "../../log/build/liblm${IMPORTED_LIB_POSTFIX}.dll.a"
			IMPORTED_LOCATION "../../log/build/liblm${IMPORTED_LIB_POSTFIX}.a"
		)
		set_target_properties(tools PROPERTIES 
			IMPORTED_IMPLIB "../../tools/build/libtools${IMPORTED_LIB_POSTFIX}.dll.a"
			IMPORTED_LOCATION "../../tools/build/libtools${IMPORTED_LIB_POSTFIX}.a"
		)
		set_target_properties(ldtools PROPERTIES 
			IMPORTED_IMPLIB "../../ldtools/build/libldtools${IMPORTED_LIB_POSTFIX}.dll.a"
			IMPORTED_LOCATION "../../ldtools/build/libldtools${IMPORTED_LIB_POSTFIX}.a"
		)
		set_target_properties(dansdl2 PROPERTIES 
			IMPORTED_IMPLIB "../../libdansdl2/build/libdansdl2${IMPORTED_LIB_POSTFIX}.dll.a"
			IMPORTED_LOCATION "../../libdansdl2/build/libdansdl2${IMPORTED_LIB_POSTFIX}.a"
		)
		set_target_properties(dfw PROPERTIES 
			IMPORTED_IMPLIB "../../dfw/build/libdfw${IMPORTED_LIB_POSTFIX}.dll.a"
			IMPORTED_LOCATION "../../dfw/build/libdfw${IMPORTED_LIB_POSTFIX}.a"
		)
		set_target_properties(env PROPERTIES 
			IMPORTED_IMPLIB "../../appenv/build/libappenv${IMPORTED_LIB_POSTFIX}.dll.a"
			IMPORTED_LOCATION "../../appenv/build/libappenv${IMPORTED_LIB_POSTFIX}.a"
		)
		set_target_properties(console PROPERTIES 
			IMPORTED_IMPLIB "../../console/build/libconsole${IMPORTED_LIB_POSTFIX}.dll.a"
			IMPORTED_LOCATION "../../console/build/libconsole${IMPORTED_LIB_POSTFIX}.a"
		)
		set_target_properties(appconsole PROPERTIES 
			IMPORTED_IMPLIB "../../appconsole/build/libappconsole${IMPORTED_LIB_POSTFIX}.dll.a"
			IMPORTED_LOCATION "../../appconsole/build/libappconsole${IMPORTED_LIB_POSTFIX}.a"
		)

		target_link_libraries(d2d_shared lm tools dansdl2 ldtools dfw env appconsole console)
	endif()

	if(${BUILD_STATIC})
		target_compile_definitions(d2d_static PUBLIC -DWINBUILD)
	endif()
else()

	set_target_properties(lm PROPERTIES IMPORTED_LOCATION "/usr/local/lib/liblm${IMPORTED_LIB_POSTFIX}.so")
	set_target_properties(tools PROPERTIES IMPORTED_LOCATION "/usr/local/lib/libtools${IMPORTED_LIB_POSTFIX}.so")
	set_target_properties(dansdl2 PROPERTIES IMPORTED_LOCATION "/usr/local/lib/libdansdl2${IMPORTED_LIB_POSTFIX}.so")
	set_target_properties(ldtools PROPERTIES IMPORTED_LOCATION "/usr/local/lib/libldtools${IMPORTED_LIB_POSTFIX}.so")
	set_target_properties(dfw PROPERTIES IMPORTED_LOCATION "/usr/local/lib/libdfw${IMPORTED_LIB_POSTFIX}.so")
	set_target_properties(env PROPERTIES IMPORTED_LOCATION "/usr/local/lib/libappenv${IMPORTED_LIB_POSTFIX}.so")
	set_target_properties(appconsole PROPERTIES IMPORTED_LOCATION "/usr/local/lib/libappconsole${IMPORTED_LIB_POSTFIX}.so")
	set_target_properties(console PROPERTIES IMPORTED_LOCATION "/usr/local/lib/libconsole${IMPORTED_LIB_POSTFIX}.so")

endif()

set(INSTALL_DIR "include/")
install(DIRECTORY include/ DESTINATION ${INSTALL_DIR} FILES_MATCHING PATTERN "*.h")


if(${BUILD_EXAMPLES})

	message("will build example")

	include_directories("${CMAKE_CURRENT_LIST_DIR}/example/include")
	set(EXAMPLE_SOURCE "")
	add_subdirectory("${PROJECT_SOURCE_DIR}/example/src")
	add_library(exlib SHARED ${EXAMPLE_SOURCE})
	set_target_properties(exlib PROPERTIES OUTPUT_NAME exlib)

	add_executable(example-app example/src/app_main.cpp)
	set_target_properties(example-app PROPERTIES OUTPUT_NAME the-dreaming-tower)

	if(${BUILD_DEBUG})

		message("example will be built as debug")
		target_compile_definitions(example-app PUBLIC "-DIS_DEBUG_BUILD")
		target_compile_definitions(exlib PUBLIC "-DIS_DEBUG_BUILD")
	endif()

	if(${BUILD_EXAMPLE_APPIMAGE})

		message("will build example as appimage for packaging...")
		target_compile_definitions(example-app PUBLIC "-DAS_APP_IMAGE")
	endif()

	if(WIN32)

		message("Doing windows build...")
		target_compile_definitions(d2d_shared PUBLIC -DWINBUILD)

		#In windows we must link libraries to each other too... We can have no unresolved symbols.
		target_link_libraries(exlib d2d_shared lm dansdl2 ldtools tools dfw env appconsole console SDL2 SDL2_ttf SDL2_mixer SDL2_image glu32 opengl32 stdc++fs)

		target_link_libraries(example-app exlib console appconsole d2d_shared env dfw tools dansdl2 ldtools lm SDL2 SDL2_ttf SDL2_mixer SDL2_image glew32 glu32 opengl32 stdc++fs)

		if(${BUILD_DEBUG}) 

			target_link_options(example-app PRIVATE -lSDL2 -mwindows)
		else()

			target_link_options(example-app PRIVATE -lSDL2 -mwindows)
		endif()
	else()

		target_link_libraries(example-app exlib d2d_static dfw tools dansdl2 ldtools env lm SDL2 SDL2_ttf SDL2_mixer SDL2_image GL stdc++fs pthread console appconsole)
	endif()

endif()

if(${BUILD_TESTS}) 

	if(WIN32)

		message("No tests for windows build")
	else() 

		message("Will build tests")

		include_directories("${CMAKE_CURRENT_LIST_DIR}/example/include")
		set(EXAMPLE_SOURCE "")
		add_executable(tile_finder_matrix tests/tile_finder_matrix.cpp)
		add_executable(persistence tests/persistence.cpp)
		add_executable(particles tests/particles.cpp)
		add_executable(aabb_static_checker tests/aabb_static_checker.cpp)

		target_link_libraries(tile_finder_matrix d2d_static dfw tools dansdl2 ldtools env lm SDL2 SDL2_ttf SDL2_mixer SDL2_image GL stdc++fs pthread)
		target_link_libraries(persistence d2d_static dfw tools dansdl2 ldtools env lm SDL2 SDL2_ttf SDL2_mixer SDL2_image GL stdc++fs pthread)
		target_link_libraries(particles d2d_static dfw tools dansdl2 ldtools env lm SDL2 SDL2_ttf SDL2_mixer SDL2_image GL stdc++fs pthread)
		target_link_libraries(aabb_static_checker d2d_static dfw tools dansdl2 ldtools env lm SDL2 SDL2_ttf SDL2_mixer SDL2_image GL stdc++fs pthread)
	endif()

endif()
