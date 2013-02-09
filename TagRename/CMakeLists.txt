Project (TagRename)

Mp3Db_Include_Directories (${XERCES_INCLUDE_DIR} ${TAGLIB_INCLUDE_DIR})

Mp3Db_Executable_Properties (
  TagRename
  PROPERTY BoostLibraries filesystem system program_options date_time
  PROPERTY OtherLibraries taglib yaml-cpp
  )
