Project (mp3db-samples)

File (GLOB cxx_files *.cxx)

Include_Directories (
  ${Boost_INCLUDE_DIR}
  ${TAGLIB_INCLUDE_DIRS}
  )

Set (samples)

ForEach (cf ${cxx_files})
  Get_Filename_Component (tgt ${cf} NAME_WE)
  List (APPEND samples ${tgt})
  Add_Executable (${tgt} ${cf})
  Link_Directories (${TAGLIB_LIBRARY_DIRS})
  Target_Link_Libraries (${tgt} ${TAGLIB_LIBRARIES})
EndForEach (cf)

Add_Custom_Target (
  samples 
  DEPENDS ${samples}
  )
