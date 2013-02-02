Project (Mp3Db)
Cmake_Minimum_Required (VERSION 2.8.4)

Set (PROJECT_LIST_FILE ${CMAKE_SOURCE_DIR}/Mp3Db.cmake)

Set (EMACS_SERVER_NAME mptag)
Include (${CMAKE_SOURCE_DIR}/CMake/Config.cmake)
Include (${CMAKE_SOURCE_DIR}/Mp3Db.cmake)

If (CMAKE_COMPILER_IS_GNUCXX)
  Set (
    CMAKE_CXX_FLAGS 
    "${CMAKE_CXX_FLAGS} -Wall -Wno-unused -Wno-inline -Woverloaded-virtual -Wmissing-declarations -Wno-write-strings"
    )
EndIf (CMAKE_COMPILER_IS_GNUCXX)

Set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

Include (${CMAKE_MODULE_PATH}/Taglib.cmake)
Include (${CMAKE_MODULE_PATH}/Zlib.cmake)

Mp3Db_Include_Directories (${Boost_INCLUDE_DIR})
Mp3Db_Include_Directories (${CMAKE_SOURCE_DIR}/Global/Include)

Add_Subdirectory (TagRename)

Set_Property (GLOBAL APPEND PROPERTY IncludeDirectories ${CMAKE_SOURCE_DIR}/Share/Data)
Set_Property (GLOBAL APPEND PROPERTY IncludeDirectories ${CMAKE_SOURCE_DIR}/Share/Data/xq)
Get_Property (IncludeDirectories GLOBAL PROPERTY IncludeDirectories)

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

Cmake_List_To_Args (AdditionalGvimSearchDirs -d ${IncludeDirectories})
Cmake_List_To_Args (ProjectModules -p ${PROJECT_MODULES})

Execute_Python (
  FILE ${PY_WRITE_EDITOR_FILE}
  ARGS -y ${CMAKE_VARIABLES_YML_OUT_FILE}
  ${EditorSearchDirectories}
)