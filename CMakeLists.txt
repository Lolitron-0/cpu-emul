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

set(SOURCES 
	"src/cpu-emul.cpp"
)

add_executable (${PROJECT_NAME} ${SOURCES})

add_subdirectory(deps/Stack)
target_include_directories(${PROJECT_NAME} PRIVATE deps/Stack/include)
