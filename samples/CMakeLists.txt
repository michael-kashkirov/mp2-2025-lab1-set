add_executable(sample_prime_numbers
    sample_prime_numbers.cpp
)

target_include_directories(sample_prime_numbers
    PRIVATE
        ${MP2_INCLUDE}
)

target_link_libraries(sample_prime_numbers
    PRIVATE
        ${MP2_LIBRARY}
)