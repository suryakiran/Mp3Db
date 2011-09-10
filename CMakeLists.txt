Project (Mp3Db)
Include (FindPkgConfig)

Cmake_Minimum_Required (VERSION 2.8.4)
Set (CMAKE_SKIP_BUILD_RPATH TRUE)

Set (PROJECT_LIST_FILE ${CMAKE_SOURCE_DIR}/Mp3Db.cmake)

Include (${CMAKE_SOURCE_DIR}/CMake/Config.cmake)
Include (${CMAKE_MODULE_PATH}/Perl.cmake)
Include (${CMAKE_SOURCE_DIR}/Mp3Db.cmake)

Check_Perl_Modules (
  Template
  XML::Simple
  File::Find::Rule
  )

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

Include (${CMAKE_MODULE_PATH}/Taglib.cmake)
Include (${CMAKE_MODULE_PATH}/Zlib.cmake)

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

Set (PROJECT_MODULES)
List (APPEND PROJECT_MODULES "TagRename")

If (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  Set (CMAKE_IS_DEBUG_BUILD "true")
Else (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  Set (CMAKE_IS_DEBUG_BUILD "false")
EndIf (${CMAKE_BUILD_TYPE} STREQUAL "Debug")

Configure_Executable_File (${MP3DB_IN_FILE} ${MP3DB_OUT_FILE} @ONLY)

Cmake_List_To_Perl_Args (AdditionalGvimSearchDirs -d ${IncludeDirectories})
Cmake_List_To_Perl_Args (ProjectModules -p ${PROJECT_MODULES})

Process_Perl_Template (
  PERL_FILE ${PL_FILE_GVIM_INIT}
  TEMPLATE ${CMAKE_GVIM_INIT_TMPL_FILE}
  OUTPUT ${CMAKE_GVIM_INIT_FILE_OUT}
  ARGS ${AdditionalGvimSearchDirs} ${ProjectModules}
  )
