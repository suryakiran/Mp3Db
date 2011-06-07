Function (MP3DB_PROJECT p_target)
  Set_Property (DIRECTORY PROPERTY ProjectName ${p_target})
  Set_Property (DIRECTORY PROPERTY ModuleName ${p_target})
  Execute_Process (COMMAND 
    ${PERL_EXECUTABLE} ${CMAKE_MODULE_PATH}/perl/CaseConv.pl
    --type=break_at_upper_case --delimiter='-' -- ${p_target}
    OUTPUT_VARIABLE p_target
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  Set_Property (DIRECTORY PROPERTY TargetName ${p_target})
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
    ${PERL_EXECUTABLE} ${CMAKE_MODULE_PATH}/perl/WriteGetPropertiesFile.pl
    --Output=${GetPropertiesFile}
    --Properties=${PropsArg}
    )
  Include (${GetPropertiesFile})
EndMacro (GET_PROPERTIES)

Function (MP3DB_EXECUTABLE)
  Get_Properties (ModuleName TargetName BoostLibraries)

  File (GLOB HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Include/${ModuleName}/*.hpp)
  File (GLOB CXX_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Src/*.cxx)
  File (GLOB UI_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Ui/*.ui)

  Set (SOURCES)

  If (UI_FILES)
    Qt4_Wrap_Ui (UI_SRCS ${UI_FILES})
    List (APPEND SOURCES ${UI_FILES})
    Mp3db_Include_Directories (${CMAKE_CURRENT_BINARY_DIR})
  EndIf (UI_FILES)

  Qt4_Wrap_Cpp (MOC_SRCS ${HPP_FILES})

  List (APPEND SOURCES ${HPP_FILES})
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

  ForEach (lib ${BoostLibraries})
    String (TOUPPER ${lib} lib) 
    List (APPEND Boost_Libraries ${Boost_@lib@_LIBRARY})
  EndForEach (lib)

  Target_Link_Libraries (
    ${TargetName}
    ${QT_QTCORE_LIBRARIES}
    ${QT_QTGUI_LIBRARIES}
    ${Boost_Libraries}
    )

EndFunction (MP3DB_EXECUTABLE)

Function (MP3DB_MODULE_PROPERTIES)
  Get_Property (ProjectName DIRECTORY PROPERTY ProjectName)
  Set (PropertiesFile ${CMAKE_CURRENT_BINARY_DIR}/${ProjectName}SetProperties.cmake)

  Set (args)
  ForEach (arg ${ARGN})
    If (${arg} STREQUAL "PROPERTY")
      List(APPEND args "--Property")
    Else (${arg} STREQUAL "PROPERTY")
      List(APPEND args ${arg})
    EndIf (${arg} STREQUAL "PROPERTY")
  EndForEach(arg)

  Execute_Process (COMMAND 
    ${PERL_EXECUTABLE} ${CMAKE_MODULE_PATH}/perl/WriteSetPropertiesFile.pl
    --Output=${PropertiesFile}
    ${args}
    )

  Include (${PropertiesFile})
EndFunction (MP3DB_MODULE_PROPERTIES)

Function (MP3DB_EXECUTABLE_PROPERTIES p_name)
  Mp3db_Project(${p_name})
  Mp3db_Module_Properties (${ARGN})
  Mp3db_Executable()
EndFunction (MP3DB_EXECUTABLE_PROPERTIES)

Function (MP3DB_INCLUDE_DIRECTORIES)
  Include_Directories (${ARGN})
  Set_Property (GLOBAL APPEND PROPERTY IncludeDirectories ${ARGN})
EndFunction (MP3DB_INCLUDE_DIRECTORIES)
