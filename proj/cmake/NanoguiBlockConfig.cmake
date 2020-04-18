if(NOT TARGET NanoguiBlock)
    get_filename_component(NanoguiBlock_PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
    get_filename_component(CINDER_PATH "${NanoguiBlock_PROJECT_ROOT}/../.." ABSOLUTE)

    file(GLOB SOURCE_LIST CONFIGURE_DEPENDS
        "${NanoguiBlock_PROJECT_ROOT}/ciNanoGui.cpp"
    )

    file(GLOB INCLUDE_LIST CONFIGURE_DEPENDS
        "${NanoguiBlock_PROJECT_ROOT}/**/*.h"
        "${NanoguiBlock_PROJECT_ROOT}/**/*.hpp"
        "${NanoguiBlock_PROJECT_ROOT}/*.h"
        "${NanoguiBlock_PROJECT_ROOT}/*.hpp"
    )

    add_library(NanoguiBlock ${SOURCE_LIST})

    target_include_directories(NanoguiBlock PUBLIC ${NanoguiBlock_PROJECT_ROOT})
    target_include_directories(NanoguiBlock SYSTEM BEFORE PUBLIC "${CINDER_PATH}/include")

    if(NOT TARGET cinder)
        include("${CINDER_PATH}/proj/cmake/configure.cmake")
        find_package(cinder REQUIRED PATHS
                "${CINDER_PATH}/${CINDER_LIB_DIRECTORY}"
                "$ENV{CINDER_PATH}/${CINDER_LIB_DIRECTORY}")
    endif()

    target_link_libraries(NanoguiBlock PRIVATE cinder)
endif()