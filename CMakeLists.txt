cmake_minimum_required(VERSION 3.0)

project(KAF2020_CHIP_8)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

#<ASMJIT>
set(ASMJIT_EMBED TRUE)
set(ASMJIT_NO_DEPRECATED TRUE)
set(ASMJIT_NO_FOREIGN TRUE)

set(ASMJIT_DIR "${CMAKE_SOURCE_DIR}/asmjit")

include("${ASMJIT_DIR}/CMakeLists.txt")

add_library(asmjit ${ASMJIT_SRC})
target_include_directories(asmjit PUBLIC ${ASMJIT_DIR}/src)
target_link_libraries(asmjit PRIVATE ${ASMJIT_DEPS})
#</ASMJIT>

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(KAF2020_CHIP_8 src/main.cpp src/types.h src/RegID.cpp src/RegID.h src/Instruction.h src/Instructions.cpp src/Instructions.h src/Instruction.cpp src/Parser.cpp src/Parser.h src/types.cpp src/Cpu.cpp src/Cpu.h src/Memory.cpp src/Memory.h src/IO.cpp src/IO.h src/CHIP8.cpp src/CHIP8.h src/SDLHelper.cpp src/SDLHelper.h src/JITSection.cpp src/JITSection.h src/JIT.cpp src/JIT.h src/constants.h )
target_link_libraries(KAF2020_CHIP_8 ${SDL2_LIBRARIES} ${ASMJIT_DEPS} asmjit Threads::Threads)

add_custom_command(TARGET KAF2020_CHIP_8
		POST_BUILD
		COMMAND /bin/sh ${CMAKE_SOURCE_DIR}/strip_debug.sh
		)
