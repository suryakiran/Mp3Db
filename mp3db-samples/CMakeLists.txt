Project (mp3db-samples)

File (GLOB cxx_files *.cxx)

Include_Directories (
  ${Boost_INCLUDE_DIR}
  ${ID3LIB_SOURCE_DIR}/include
  )

Set (samples)

Get_Property (id3lib DIRECTORY ${ID3LIB_SOURCE_DIR}/src PROPERTY Target_Name)

ForEach (cf ${cxx_files})
  Get_Filename_Component (tgt ${cf} NAME_WE)
  List (APPEND samples ${tgt})
  Add_Executable (${tgt} ${cf})
  Target_Link_Libraries (${tgt} ${id3lib})
EndForEach (cf)

Add_Custom_Target (
  samples 
  DEPENDS ${samples}
  )
