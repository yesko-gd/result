add_library(result)

target_link_libraries(result
    PUBLIC
        dynamic_string
        stb_ds
)

target_sources(result
    PRIVATE
        result.c

    PUBLIC
        FILE_SET HEADERS
        FILES result.h
)
