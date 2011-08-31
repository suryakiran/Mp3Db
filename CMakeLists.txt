Project (Mp3Db)
Include (FindPkgConfig)

Cmake_Minimum_Required (VERSION 2.8.4)
Set (CMAKE_SKIP_BUILD_RPATH TRUE)

Include (${CMAKE_SOURCE_DIR}/CMake/Dirs.cmake)
Include (${CMAKE_MODULE_PATH}/Macros.cmake)
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

File (GLOB ConfigInFiles ${MP3DB_CONFIG_DIR}/*.in)

Set (AlternateDirs)

List (APPEND AlternateDirs \"${MP3DB_CONFIG_DIR}\")
List (APPEND AlternateDirs \"${CMAKE_BINARY_DIR}\")

Execute_Process (COMMAND 
  ${PERL_EXECUTABLE} ${CMAKE_PERL_DIR}/WritePythonList.pl
  ${AlternateDirs}
  OUTPUT_VARIABLE CONFIG_ALTERNATE_DIRS
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )

ForEach (cf ${ConfigInFiles})
  Set (CONFIG_IN_FILE ${cf})
  Get_Filename_Component (of ${cf} NAME)
  String (REGEX REPLACE "\\.in$" "" of ${of})
  Set (CONFIG_OUT_FILE ${CMAKE_BINARY_DIR}/${of})
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
