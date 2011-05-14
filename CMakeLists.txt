Project (Mp3Db)
Include (FindPkgConfig)

Cmake_Minimum_Required (VERSION 2.8.4)

Set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

Set(Boost_ADDITIONAL_VERSIONS "1.46.1")

If (CMAKE_COMPILER_IS_GNUCXX)
	Set (
		CMAKE_CXX_FLAGS 
		"${CMAKE_CXX_FLAGS} -Wall -Wno-unused -Wno-inline -Woverloaded-virtual -Wmissing-declarations -Wno-write-strings"
		)
EndIf (CMAKE_COMPILER_IS_GNUCXX)

Set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

Find_Package (Boost)

Pkg_Check_Modules (TAGLIB REQUIRED taglib)
Pkg_Check_Modules (ZLIB REQUIRED zlib)

Add_Subdirectory (mp3db-samples)
