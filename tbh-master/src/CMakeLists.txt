cmake_minimum_required(VERSION 3.16)

add_compile_definitions(_USE_MATH_DEFINES)

add_executable(tbh WIN32 main.cpp pattern.cpp level.cpp)
set_property(TARGET tbh PROPERTY CXX_STANDARD 20)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(tbh PRIVATE sfml-main sfml-graphics)