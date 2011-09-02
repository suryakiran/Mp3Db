Project (Mp3Db)
Include (FindPkgConfig)

Cmake_Minimum_Required (VERSION 2.8.4)
Set (CMAKE_SKIP_BUILD_RPATH TRUE)

Include (${CMAKE_SOURCE_DIR}/CMake/Config.cmake)
Include (${CMAKE_MODULE_PATH}/Perl.cmake)
Include (${CMAKE_SOURCE_DIR}/Mp3Db.cmake)

If (CMAKE_COMPILER_IS_GNUCXX)
	Set (
		CMAKE_CXX_FLAGS 
		"${CMAKE_CXX_FLAGS} -Wall -Wno-unused -Wno-inline -Woverloaded-virtual -Wmissing-declarations -Wno-write-strings"
		)
EndIf (CMAKE_COMPILER_IS_GNUCXX)

Set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

Find_Package (Boost
  COMPONENTS
  filesystem date_time system program_options
  )
Find_Package (Qt4 COMPONENTS QtCore QtGui)
Include (${QT_USE_FILE})

Pkg_Check_Modules (TAGLIB REQUIRED taglib)
Pkg_Check_Modules (ZLIB REQUIRED zlib)

Mp_Include_Dirs (${Boost_INCLUDE_DIR})

Add_Subdirectory (TagRename)
#Add_Subdirectory (Samples)

Get_Property (IncludeDirectories GLOBAL PROPERTY IncludeDirectories)

ForEach (id ${IncludeDirectories})
  Set (
    ADDITIONAL_GVIM_SEARCH_DIRS 
    "${ADDITIONAL_GVIM_SEARCH_DIRS}\nset path+=${id}"
    )
EndForEach (id)

Set (AlternateDirs)

List (APPEND AlternateDirs \"${PROJECT_CONFIG_DIR}\")
List (APPEND AlternateDirs \"${CMAKE_BINARY_DIR}\")

Set (PROJECT_MODULES "'TagRename'")
Configure_Executable_File (${PROJECT_GVIM_INIT_FILE} ${CMAKE_GVIM_INIT_FILE_OUT} @ONLY)
Configure_Executable_File (${MP3DB_IN_FILE} ${MP3DB_OUT_FILE} @ONLY)
