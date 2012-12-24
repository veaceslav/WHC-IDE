cmake_minimum_required(VERSION 2.6)
project(MyCL)

#Linux
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	SET (EXTRA_LIBS ${EXTRA_LIBS} OpenCL)
	SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/opt/AMDAPP/include -L/opt/AMDAPP/lib/x86_64/")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

#Windows support
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	INCLUDE_DIRECTORIES("C:\\Program Files\\NVIDIA\ GPU\ Computing\ Toolkit\\CUDA\\v4.2\\include")
	LINK_DIRECTORIES("C:\\Program Files\\NVIDIA\ GPU\ Computing\ Toolkit\\CUDA\\v4.2\\lib\\Win32")
	SET (EXTRA_LIBS ${EXTRA_LIBS} OpenCL.lib)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

add_executable(Task1 main.cpp)
target_link_libraries(Task1 ${EXTRA_LIBS})
