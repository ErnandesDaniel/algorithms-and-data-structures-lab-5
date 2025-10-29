cmake_minimum_required(VERSION 4.0)
project(algorithms_and_data_structures_lab_4)

set(CMAKE_CXX_STANDARD 17)

# =============================
# 1. НАСТРОЙКА clang-format (форматирование перед сборкой)
# =============================

# Укажи путь вручную, если find_program не работает
#set(CLANG_FORMAT_EXE "C:/Program Files/LLVM/bin/clang-format.exe")

# ИЛИ попробуй найти автоматически (если PATH настроен):
find_program(CLANG_FORMAT_EXE NAMES clang-format REQUIRED)

# У тебя только main.cpp — можно расширить список при необходимости
set(SOURCE_FILES zero-lab-main.cpp)

# Цель: автоформатирование
add_custom_target(format-code
        COMMAND ${CLANG_FORMAT_EXE} -i ${SOURCE_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Formatting code with clang-format..."
)

# =============================
# 2. НАСТРОЙКА clang-tidy (линтинг перед компиляцией)
# =============================

# Включаем clang-tidy для всех последующих целей
# Будет использовать .clang-tidy из корня проекта
#set(CMAKE_CXX_CLANG_TIDY "clang-tidy;--extra-arg=-std=c++17")

# =============================
# 3. СОБИРАЕМ ИСПОЛНЯЕМЫЙ ФАЙЛ
# =============================

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# =============================
# 4. ЗАВИСИМОСТЬ: СНАЧАЛА ФОРМАТИРОВАНИЕ → ПОТОМ СБОРКА
# =============================

add_dependencies(${PROJECT_NAME} format-code)




