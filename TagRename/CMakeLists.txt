Project (TagRename)

Mp3Db_Include_Directories (${XERCES_INCLUDE_DIR} ${TAGLIB_INCLUDE_DIR})
Include_Directories(${INTEL_TBB_INCLUDE_DIR})

Mp3Db_Executable_Properties (
  TagRename
  PROPERTY BoostLibraries filesystem system program_options date_time regex locale
  PROPERTY OtherLibraries taglib yaml-cpp curl
  )

Get_Property(TargetName DIRECTORY PROPERTY TargetName)
Target_Link_Libraries(
  ${TargetName}
  ${INTEL_TBB_LIBRARIES}
  )