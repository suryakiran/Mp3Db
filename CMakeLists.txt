Project (Mp3Db)
Include (FindPkgConfig)

Cmake_Minimum_Required (VERSION 2.8.4)
Set (CMAKE_SKIP_BUILD_RPATH TRUE)

Set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake)
Set (MP3DB_CMAKE_MODULES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeUtils)
Set (MP3DB_CONFIG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Config)

Include (FindPerl)
Include (${MP3DB_CMAKE_MODULES}/Mp3Db.cmake)

Set(Boost_ADDITIONAL_VERSIONS "1.46.1")

If (CMAKE_COMPILER_IS_GNUCXX)
	Set (
		CMAKE_CXX_FLAGS 
		"${CMAKE_CXX_FLAGS} -Wall -Wno-unused -Wno-inline -Woverloaded-virtual -Wmissing-declarations -Wno-write-strings"
		)
EndIf (CMAKE_COMPILER_IS_GNUCXX)

Set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

Find_Package (Boost)
Find_Package (Qt4 COMPONENTS QtCore QtGui)
Include (${QT_USE_FILE})

Pkg_Check_Modules (TAGLIB REQUIRED taglib)
Pkg_Check_Modules (ZLIB REQUIRED zlib)

Include_Directories (${Boost_INCLUDE_DIR})

Add_Subdirectory (TagRename)
#Add_Subdirectory (Samples)

File (GLOB ConfigInFiles ${MP3DB_CONFIG_DIR}/*.in)

ForEach (cf ${ConfigInFiles})
  Get_Filename_Component (of ${cf} NAME_WE)
  Set (of ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${of})
  Configure_File (${cf} ${of} @ONLY)
  File (
    COPY ${of} DESTINATION ${CMAKE_BINARY_DIR}
    FILE_PERMISSIONS 
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
    )
EndForEach (cf)