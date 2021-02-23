file(GLOB file_list "${CMAKE_CURRENT_SOURCE_DIR}/[a-z]*.cpp")

foreach(fname IN LISTS file_list)
  get_filename_component(bname ${fname} NAME_WE)
  add_executable(${bname} ${fname})
  if ("${LIBRARIES}")
    target_link_libraries(${bname} ${LIBRARIES})
  endif()
endforeach()
