# add modules
set(subdirs assembler;vm_translater)
foreach(dir IN LISTS subdirs)
  file(GLOB file_list "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/[a-z]*.cpp")
  if (file_list)
    add_library(${dir} ${file_list})
    set(LIBRARIES ${LIBRARIES} ${dir} CACHE INTERNAL "")
  endif()
endforeach()

# add executables
add_subdirectory(snipets)
