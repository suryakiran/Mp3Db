Set (GUI_DIR_Debug ${INSTALL_PREFIX_Debug}/gui)
Set (GUI_DIR_Release ${INSTALL_PREFIX_Release}/gui)
Set (PERL_MODULES_DIR_Release ${INSTALL_PREFIX_Release}${PERL_ARCH_DIR_SUFFIX}/Mp3Db)
Set (PERL_MODULES_DIR_Debug ${INSTALL_PREFIX_Debug}${PERL_ARCH_ARCH_DIR_SUFFIX}/Mp3Db)
Set (PLUGINS_DIR_Debug ${INSTALL_PREFIX_Debug}/plugins)
Set (PLUGINS_DIR_Release ${INSTALL_PREFIX_Release}/plugins)
Set (SHARE_DATA_DIR_Debug ${INSTALL_PREFIX_Debug}/share/data)
Set (SHARE_DATA_DIR_Release ${INSTALL_PREFIX_Release}/share/data)
Set (SHARE_DATA_DIR ${CMAKE_SOURCE_DIR}/Share/Data)
Set (AppPrefix Mp3Db)

Include (${CMAKE_MODULE_PATH}/Module.cmake)

Function (MP_INCLUDE_DIRS)
  Include_Directories (${ARGN})
  ForEach (dir ${ARGN})
    Get_Filename_Component (dir ${dir} ABSOLUTE)
    String (REPLACE "Include" "Src" src_dir ${dir})
    Set_Property (GLOBAL APPEND PROPERTY MPIncludeDirs ${dir})
    If (NOT ${src_dir} STREQUAL ${dir})
      Set_Property (GLOBAL APPEND PROPERTY MPSrcDirs ${src_dir})
    EndIf (NOT ${src_dir} STREQUAL ${dir})
  EndForEach (dir)
EndFunction (MP_INCLUDE_DIRS)

Function (MP3DB_MODULE_PROPERTIES)
  Get_Property (ProjectName DIRECTORY PROPERTY ProjectName)
  Set (PropertiesFile ${CMAKE_CURRENT_BINARY_DIR}/${ProjectName}SetProperties.cmake)

  Set (get_property_name 0)
  Set (get_property_value 0)

  Set (keys)
  Set (key)
  ForEach (arg ${ARGN})
    If (${arg} STREQUAL "PROPERTY")
      Set (get_property_name 1)
      Set (args)
      Set (key)
      Set (get_property_value 0)
    ElseIf (get_property_name)
      Set (key ${arg})
      Set (values_${key})
      List (APPEND keys ${key})
      Set (get_property_name 0)
      Set (get_property_value 1)
    ElseIf (get_property_value)
      If (key)
        List (APPEND values_${key} "-v${arg}")
      EndIf (key)
    EndIf (${arg} STREQUAL "PROPERTY")
  EndForEach(arg)

  File (REMOVE ${PropertiesFile})
  ForEach (key ${keys})
    Execute_Python (
      FILE ${CMAKE_PYTHON_DIR}/WriteSetProperties.py
      ARGS -n ${key} ${values_${key}}
      CMAKE_OUTPUT ${PropertiesFile}
      )
  EndForEach (key)
  
EndFunction (MP3DB_MODULE_PROPERTIES)

Function (MP3DB_PROJECT p_target)
  Set_Property (DIRECTORY PROPERTY ProjectName ${p_target})
  Set_Property (DIRECTORY PROPERTY ModuleName ${p_target})
  Execute_Python (
    FILE ${PY_COMMANDS_FILE}
    ARGS -c split_string_on_upper_case ${p_target}
    OUTPUT p_target_name
    )
  Set_Property (DIRECTORY PROPERTY TargetName ${p_target_name})
EndFunction (MP3DB_PROJECT)

Macro (GET_PROPERTIES)
  Set (GetPropertiesFile
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}GetProperties.cmake)
  Set (PropsArg)
  ForEach (arg ${ARGN})
    If (NOT PropsArg)
      Set (PropsArg ${arg})
    Else (NOT PropsArg)
      Set (PropsArg ${PropsArg},${arg})
    EndIf (NOT PropsArg)
  EndForEach (arg)
  Execute_Process (COMMAND
    ${PERL_EXECUTABLE} ${CMAKE_MODULE_PATH}/Perl/WriteGetPropertiesFile.pl
    --Output=${GetPropertiesFile}
    --Properties=${PropsArg}
    )
  Include (${GetPropertiesFile})
EndMacro (GET_PROPERTIES)


Function (MP3DB_EXECUTABLE)
  Get_Properties (ModuleName TargetName BoostLibraries OtherLibraries)

  Set (INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Include/${ModuleName})
  Set (SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Src)
  Set (UI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Ui)

  File (GLOB HXX_FILES ${INC_DIR}/*.hxx)
  File (GLOB CXX_FILES ${SRC_DIR}/*.cxx)
  File (GLOB UI_FILES ${UI_DIR}/*.ui)

  Set_Property (GLOBAL APPEND PROPERTY IncludeDirectories ${INC_DIR})
  Set_Property (GLOBAL APPEND PROPERTY IncludeDirectories ${SRC_DIR})

  Set (SOURCES)

  If (UI_FILES)
    Qt4_Wrap_Ui (UI_SRCS ${UI_FILES})
    List (APPEND SOURCES ${UI_FILES})
    Mp3db_Include_Directories (${CMAKE_CURRENT_BINARY_DIR})
  EndIf (UI_FILES)

  Qt_Get_Moc_Files (MOC_SRCS ${HXX_FILES})

  List (APPEND SOURCES ${HXX_FILES})
  List (APPEND SOURCES ${CXX_FILES})
  List (APPEND SOURCES ${UI_SRCS})
  List (APPEND SOURCES ${MOC_SRCS})

  Mp3db_Include_Directories (
    ${CMAKE_CURRENT_SOURCE_DIR}/Include
    ${QT_QTCORE_INCLUDE_DIR}
    ${QT_QTGUI_INCLUDE_DIR}
    )

  Add_Executable (
    ${TargetName}
    ${SOURCES}
    )

  Set (Boost_Libraries)
  Set (Other_Libraries)

  ForEach (lib ${BoostLibraries})
    String (TOUPPER ${lib} lib) 
    List (APPEND Boost_Libraries ${Boost_@lib@_LIBRARY})
  EndForEach (lib)

  ForEach (lib ${OtherLibraries})
    String (REPLACE "-" "_" lib ${lib})
    String (TOUPPER ${lib} lib) 
    List (APPEND Other_Libraries ${@lib@_LIBRARIES})
  EndForeach (lib)

  Target_Link_Libraries (
    ${TargetName}
    ${QT_QTCORE_LIBRARIES}
    ${QT_QTGUI_LIBRARIES}
    ${Boost_Libraries}
    ${Other_Libraries}
    )
EndFunction (MP3DB_EXECUTABLE)

Function (MP3DB_EXECUTABLE_PROPERTIES p_name)
  Mp3db_Project(${p_name})
  Mp3db_Module_Properties (${ARGN})
  Mp3db_Executable()
EndFunction (MP3DB_EXECUTABLE_PROPERTIES)

Function (MP3DB_INCLUDE_DIRECTORIES)
  Include_Directories (${ARGN})
  Set_Property (GLOBAL APPEND PROPERTY IncludeDirectories ${ARGN})
EndFunction (MP3DB_INCLUDE_DIRECTORIES)

Get_Filename_Component(MP_CONFIG_DIR ${CMAKE_SOURCE_DIR}/Config ABSOLUTE)
Set (DLL_UTILITIES_DIR ${CMAKE_CURRENT_BINARY_DIR}/DllUtilities)

Find_File_In_Dir (MP3DB_IN_FILE mp3db.py.in ${PROJECT_CONFIG_DIR})
Set (MP3DB_OUT_FILE ${CMAKE_BINARY_DIR}/mp3db)

Mark_As_Advanced (
  MP3DB_IN_FILE
  GVIMINIT_IN_FILE
  USE_BOOST
  USE_POCO
  USE_QT
  )

Function (Include_Zorba)
  Mp3db_Include_Directories (
    ${ZORBA_INCLUDE_DIR}
    )
  
  If (UNIX)
    Remove_Definitions ("-DUNIX")
  Endif (UNIX)
EndFunction (Include_Zorba)
