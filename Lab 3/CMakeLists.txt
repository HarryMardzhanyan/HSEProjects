cmake_minimum_required(VERSION 3.10)
project(Lab3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Общие флаги компилятора
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(GLOBAL_WARNINGS "-Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wunused")
endif()

# Создаём библиотеку из общих исходников
add_library(calc_lib
    src/ast.cpp
    src/lexer.cpp
    src/parser.cpp
    src/visitor.cpp
)

# Добавляем заголовочные файлы для библиотеки
target_include_directories(calc_lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external
)

# Основной исполняемый файл
add_executable(calculator src/main.cpp)
target_link_libraries(calculator calc_lib)


# Тесты
add_executable(tests tests/test.cpp)
target_link_libraries(tests calc_lib)

# Тестирование
enable_testing()
add_test(NAME Catch2Tests COMMAND tests)