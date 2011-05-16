Project (TagRename)
Mp3db_Project (TagRename)

Mp3Db_Executable ()
Include_Directories (${CMAKE_CURRENT_SOURCE_DIR}/Include)

File (GLOB CXX_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Src/*.cxx)

Add_Executable (${TargetName} ${CXX_FILES})
