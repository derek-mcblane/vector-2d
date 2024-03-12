cmake_minimum_required(VERSION 3.25.0)

project(Vector VERSION 0.1.1)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

set(Vector_WARNING_OPTIONS
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wunused>>"
    "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

add_compile_definitions(
    $<$<CONFIG:DEBUG>:Vector_DEBUG>
)

add_subdirectory(source)

option(Vector_ENABLE_TESTING "Enable testing for Vector" ON)
if (${Vector_ENABLE_TESTING})
    enable_testing()
    add_subdirectory(tests)
endif()

export(EXPORT VectorTargets
    NAMESPACE Vector::
)
install(EXPORT VectorTargets
    NAMESPACE Vector::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Vector
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/VectorConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion 
)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/VectorConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/VectorConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Vector
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/VectorConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/VectorConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Vector
)

export(PACKAGE Vector)

include(CPack)
