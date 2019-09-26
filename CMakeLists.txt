cmake_minimum_required(VERSION 3.0)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if(DEFINED ENV{VITASDK})
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
    else()
        message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
    endif()
endif()

project(VitaUDPPlayground)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_TITLEID "JAGJ00002")
set(VITA_VERSION  "00.01")

set(CFLAGS "-Wl,-q -Wall -O3")

include_directories(
    third_party/libdebugnet/include/
    third_party/debugScreen
)

add_executable(${PROJECT_NAME}.elf 
    src/main.c
    third_party/libdebugnet/source/debugnet.c
    third_party/debugScreen/debugScreen.c
)
target_compile_options(${PROJECT_NAME}.elf PRIVATE ${CFLAGS})

target_link_libraries(${PROJECT_NAME}.elf
        c
	SceNetCtl_stub
	SceNet_stub
	SceSysmodule_stub
	SceCtrl_stub
	SceDisplay_stub
	)

vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME}.elf)
vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
    VERSION ${VITA_VERSION}
	NAME ${PROJECT_NAME}
)
