add_library(usermod_qrcode INTERFACE)

target_sources(usermod_qrcode INTERFACE
	${CMAKE_CURRENT_LIST_DIR}/../c/qrcodegen.c
	${CMAKE_CURRENT_LIST_DIR}/qrcode.c
)

target_include_directories(usermod_qrcode INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../c/
)

target_compile_definitions(usermod_qrcode INTERFACE
    MODULE_QRCODE_ENABLED=1
)

target_link_libraries(usermod INTERFACE usermod_qrcode)
