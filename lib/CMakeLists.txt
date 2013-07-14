include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5Xml_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS} ${Qt5Xml_DEFINITIONS})

set(iwf_SOURCES
    iwf.cpp
    ../include/iwf.h
)

add_library(iwf SHARED ${iwf_SOURCES})
target_link_libraries(iwf ${Qt5Widgets_LIBRARIES})

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

