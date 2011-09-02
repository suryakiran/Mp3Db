Project (TagRename)

Mp3Db_Executable_Properties (
  TagRename
  PROPERTY "BoostLibraries=filesystem,system,program_options"
  )

Target_Link_Libraries (
  tag-rename ${TAGLIB_LIBRARIES}
  )
