Function (MP3DB_PROJECT p_target)
  Set_Property (DIRECTORY PROPERTY ModuleName ${p_target})
  Execute_Process (COMMAND 
    ${PERL_EXECUTABLE} ${CMAKE_MODULE_PATH}/perl/CaseConv.pl
    --type=break_at_upper_case --delimiter='-' -- ${p_target}
    OUTPUT_VARIABLE p_target
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  Set_Property (DIRECTORY PROPERTY TargetName ${p_target})
EndFunction (MP3DB_PROJECT)

Function (MP3DB_EXECUTABLE)
  Get_Property (ModuleName DIRECTORY PROPERTY ModuleName)
  Get_Property (TargetName DIRECTORY PROPERTY TargetName)

  File (GLOB HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Include/${ModuleName}/*.hpp)
  File (GLOB CXX_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Src/*.cxx)
  File (GLOB UI_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Ui/*.ui)

  Set (SOURCES)

  If (UI_FILES)
    Qt4_Wrap_Ui (UI_SRCS ${UI_FILES})
    List (APPEND SOURCES ${UI_FILES})
    Include_Directories (${CMAKE_CURRENT_BINARY_DIR})
  EndIf (UI_FILES)

  Qt4_Wrap_Cpp (MOC_SRCS ${HPP_FILES})

  List (APPEND SOURCES ${HPP_FILES})
  List (APPEND SOURCES ${CXX_FILES})
  List (APPEND SOURCES ${UI_SRCS})
  List (APPEND SOURCES ${MOC_SRCS})

  Include_Directories (${CMAKE_CURRENT_SOURCE_DIR}/Include)

  Include_Directories (${QT_QTCORE_INCLUDE_DIR})
  Include_Directories (${QT_QTGUI_INCLUDE_DIR})


  Add_Executable (
    ${TargetName}
    ${SOURCES}
    )

  Target_Link_Libraries (
    ${TargetName}
    ${QT_QTCORE_LIBRARIES}
    ${QT_QTGUI_LIBRARIES}
    )

EndFunction (MP3DB_EXECUTABLE)
