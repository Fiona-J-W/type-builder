project(dlo)
cmake_minimum_required(VERSION 2.8)

#we need C++11:
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Weffc++" )

if(CMAKE_BUILD_TYPE MATCHES "Debug")
	message("creating Debug-Version")
	add_definitions( -DDEBUG )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g" )
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
	message("creating Release-Version")
	add_definitions( -DNDEBUG )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
	if( CMAKE_COMPILER_IS_GNUCXX )
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3"  )
	elseif( CMAKE_COMPILER_IS_CLANGXX )
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O4"  )
	endif()
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(src)
