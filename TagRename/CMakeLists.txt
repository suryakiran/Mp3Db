Project (TagRename)

Include_Zorba()
Mp3Db_Include_Directories (
  ${XERCES_INCLUDE_DIR}
  )

Mp3Db_Executable_Properties (
  TagRename
  PROPERTY "BoostLibraries=filesystem,system,program_options"
  PROPERTY "OtherLibraries=taglib,xerces"
  )
