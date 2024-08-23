

#include <iostream>

typedef struct unit_test {
    const char *test_name;
    bool (*test_func_ptr)(void);
} unit_test_t;

#define TEST_CASE(fnc_ptr) {#fnc_ptr, fnc_ptr}

#define ASSERT_TRUE(x) if (!x) {std::cout << "FAILED line " << __LINE__ << " expected true, was false" << std::endl; return false;}
#define ASSERT_FALSE(x) if (x) {std::cout << "FAILED line " << __LINE__ << " expected false, was true" << std::endl; return false;}
#define ASSERT_INT_EQUALS(exp, act) if (act != exp) {std::cout << "FAILED line " << __LINE__ << " expected " << \
                                        exp << ", was " << act << std::endl; return false;}

#define MAIN(test_list) \
int main()                                                                                                                \
{                                                                                                                         \
    int test_idx = 0;                                                                                                     \
    int num_failures = 0;                                                                                                 \
    int total_test_num = 0;                                                                                               \
                                                                                                                          \
    /* count total number of unit tests before starting */                                                                \
    while (test_list[total_test_num].test_name) {                                                                         \
        total_test_num++;                                                                                                 \
    }                                                                                                                     \
                                                                                                                          \
    std::cout << std::endl;                                                                                               \
                                                                                                                          \
    while (test_list[test_idx].test_name)                                                                                 \
    {                                                                                                                     \
        std::cout << "Test [" << (test_idx+1) << "/" << total_test_num << "]: " << test_list[test_idx].test_name << ": "; \
        int rc = test_list[test_idx].test_func_ptr();                                                                     \
        if (rc) {                                                                                                         \
            std::cout << "PASSED" << std::endl;                                                                           \
        } else {                                                                                                          \
            num_failures++;                                                                                               \
        }                                                                                                                 \
                                                                                                                          \
        test_idx++;                                                                                                       \
    }                                                                                                                     \
                                                                                                                          \
    if (num_failures > 0) {                                                                                               \
        std::cout << std::endl << num_failures << " out of " << total_test_num << " failed" << std::endl;                 \
        return 1;                                                                                                         \
    }                                                                                                                     \
                                                                                                                          \
    std::cout << "All test passed" << std::endl;                                                                          \
    return 0;                                                                                                             \
}                                                                                                                         \
