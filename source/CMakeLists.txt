add_library(Vector2D INTERFACE)
target_compile_features(Vector2D INTERFACE  cxx_std_20)
target_include_directories(Vector2D INTERFACE ${CMAKE_CURRENT_LIST_DIR})
