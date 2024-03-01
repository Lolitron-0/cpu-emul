cmake_minimum_required (VERSION 3.15)

# MSVC Hot-reload
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("cpu-emul")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS --coverage)
endif()

set(SOURCES 
	src/main.cpp
	src/Executor.cpp
    src/RuntimeContext.cpp
	src/FileParser.cpp
	src/commands/CommandBase.cpp
)

add_executable (${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE src)

configure_file(examples/fib_loop.pasm examples/fib_loop.pasm)
configure_file(examples/fib_no_loop.pasm examples/fib_no_loop.pasm)

add_subdirectory(deps/Stack)
target_link_libraries(${PROJECT_NAME} PRIVATE Stack)

add_subdirectory(deps/cul)
target_link_libraries(${PROJECT_NAME} PRIVATE cul)

find_package(Boost COMPONENTS program_options REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::program_options)
