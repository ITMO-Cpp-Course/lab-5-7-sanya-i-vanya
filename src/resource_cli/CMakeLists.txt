add_executable(document_cli
        src/main.cpp
)

target_link_libraries(document_cli PRIVATE body_core project_warnings)
if (TARGET project_sanitizers)
    target_link_libraries(document_cli PRIVATE project_sanitizers)
endif ()

