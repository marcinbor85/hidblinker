cmake_minimum_required( VERSION 3.0 )

project( lib-usbhid-blinker LANGUAGES C )

set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

include_directories( ${PROJECT_SOURCE_DIR}/src )

file( GLOB SRC_FILES src/*.c )
add_library( hidblinker SHARED ${SRC_FILES} )
target_link_libraries( hidblinker usb-1.0 )
set_target_properties( hidblinker PROPERTIES VERSION 0.1.0 SOVERSION 1 )

install( TARGETS hidblinker DESTINATION lib )
install( FILES src/hidblinker.h DESTINATION include )

add_executable( example example/main.c )
target_link_libraries( example hidblinker )

add_custom_target( cleanall COMMAND rm -rf Makefile CMakeCache.txt CMakeFiles/ bin/ lib/ cmake_install.cmake CTestTestfile.cmake Testing/ )
add_custom_target( uninstall COMMAND xargs rm < install_manifest.txt )
