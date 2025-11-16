add_library(${MP2_LIBRARY}
    tbitfield.cpp
    tset.cpp
)

target_include_directories(${MP2_LIBRARY}
    PRIVATE
        ${MP2_INCLUDE}
)