Project (TagRename)

Include_Zorba()
Mp3Db_Include_Directories (
  ${XERCES_INCLUDE_DIR}
  )

Mp3Db_Executable_Properties (
  TagRename
  PROPERTY "BoostLibraries=filesystem,system,program_options"
  )

Target_Link_Libraries (
  tag-rename ${TAGLIB_LIBRARIES} ${ZORBA_LIBRARIES} ${XERCES_DEBUG_LIBRARY}
  )
