include(ExternalProject)

ExternalProject_Add(
    OpenNURBS
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/OpenNURBS
    DEPENDS ZLIB
    GIT_REPOSITORY https://github.com/darbyjohnston/opennurbs
    GIT_TAG origin/zlib
    CMAKE_ARGS
        -DCMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
        -DCMAKE_PREFIX_PATH=${CMAKE_INSTALL_PREFIX}
        -DCMAKE_POLICY_DEFAULT_CMP0091:STRING=NEW
        -DCMAKE_MSVC_RUNTIME_LIBRARY:STRING=MultiThreaded$<$<CONFIG:Debug>:Debug>DLL
        -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
        -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
        -DBUILD_SHARED_LIBS=${OpenNURBS_SHARED_LIBS}
        -DZLIB_SHARED_LIBS=${ZLIB_SHARED_LIBS})

