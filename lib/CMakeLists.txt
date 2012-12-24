set(QT_DONT_USE_QTGUI)

include(${QT_USE_FILE})
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
add_definitions(${QT_DEFINITIONS})

set(iwf_SOURCES
    iwf.cpp
    ../include/iwf.h
)

add_library(iwf SHARED ${iwf_SOURCES})
target_link_libraries(iwf ${QT_LIBRARIES})

if(UNIX)
    install(TARGETS iwf
        LIBRARY DESTINATION lib
    )
endif(UNIX)

if(WIN32 AND MINGW)
    install(TARGETS iwf
        RUNTIME DESTINATION .
    )
endif(WIN32 AND MINGW)

