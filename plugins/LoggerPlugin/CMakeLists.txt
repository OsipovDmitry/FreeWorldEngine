set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

include_directories(../../3rdparty)
include_directories(../../include)
link_directories(${LIBRARY_OUTPUT_PATH})

set(HEADERS ../../include/ILog.h
            ../../include/ILogger.h
			TerminalLog.h
			HtmlLog.h
            LoggerPlugin.h)

set(SOURCES TerminalLog.cpp
			HtmlLog.cpp
			LoggerPlugin.cpp
            main.cpp)

add_library(LoggerPlugin SHARED ${HEADERS} ${SOURCES})
	
target_link_libraries(LoggerPlugin CoreEngine)