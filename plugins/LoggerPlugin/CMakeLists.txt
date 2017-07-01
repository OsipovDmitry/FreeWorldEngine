set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

include_directories(../../include)
link_directories(${LIBRARY_OUTPUT_PATH})

set(HEADERS
	TerminalLog.h
	HtmlLog.h
	TextFileLog.h
	LoggerPlugin.h)

set(INTERFACE_HEADERS
	../../include/core/ILog.h
	../../include/core/ILogger.h)

source_group("Interface Headers" FILES ${INTERFACE_HEADERS})

set(SOURCES
	TerminalLog.cpp
	HtmlLog.cpp
	TextFileLog.cpp
	LoggerPlugin.cpp
	main.cpp)

add_library(LoggerPlugin SHARED ${HEADERS} ${INTERFACE_HEADERS} ${SOURCES})
	
target_link_libraries(LoggerPlugin CoreEngine)
target_link_libraries(LoggerPlugin FWEUtility)
