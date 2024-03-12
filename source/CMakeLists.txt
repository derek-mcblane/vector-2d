add_library(Vector INTERFACE)
add_library(Vector::Vector ALIAS Vector)
target_compile_features(Vector INTERFACE cxx_std_20)
target_include_directories(Vector INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_sources(Vector
INTERFACE FILE_SET HEADERS FILES
    vec.h
)

install(TARGETS Vector EXPORT VectorTargets
    FILE_SET HEADERS INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
