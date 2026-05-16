include(FetchContent)

FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.11.0
)
FetchContent_MakeAvailable(catch2)

add_executable(resource_tests tests.cpp)

# Путь к библиотеке
target_link_directories(resource_tests PRIVATE ${CMAKE_BINARY_DIR}/src)

# Путь к заголовкам
target_include_directories(resource_tests PRIVATE
        ${CMAKE_SOURCE_DIR}/src/resource_core/include
        ${CMAKE_SOURCE_DIR}/src/include
)

# Линковка (латинские скобки!)
target_link_libraries(resource_tests PRIVATE
        resource_core
        project_warnings
        Catch2::Catch2WithMain
)

if (TARGET project_sanitizers)
    target_link_libraries(resource_tests PRIVATE project_sanitizers)
endif()

add_test(NAME resource_tests COMMAND resource_tests)