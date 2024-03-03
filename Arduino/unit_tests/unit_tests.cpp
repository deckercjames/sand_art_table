// Your First C++ Program

#include <iostream>

#include "path_calculator.h"

#define ASSERT_FAILURE do {;} while(0)
#define ASSERT_TRUE(x) if (!x) {std::cout << "FAILED line " << __LINE__ << " expected true, was false" << std::endl; return false;}
#define ASSERT_FALSE(x) if (x) {std::cout << "FAILED line " << __LINE__ << " expected false, was true" << std::endl; return false;}
#define ASSERT_INT_EQUALS(exp, act) if (act != exp) {std::cout << "FAILED line " << __LINE__ << " expected " << exp << ", was " << act << std::endl; return false;}

// Redefine the stepper motor directions for implementation
// Don't include the arduino header so we can unit test easier
#define FORWARD 1
#define BACKWARD 2


// One move adjacent tests

bool test_move_adjacent_up()
{
    current_pos_x = 10;
    current_pos_y = 10;
    set_target_pos(10, 11);
    ASSERT_FALSE(at_target());
    move_one_instruction_t move_instr;
    move_toward_target(&move_instr);
    ASSERT_TRUE(at_target());
    ASSERT_INT_EQUALS(1, move_instr.steps_1);
    ASSERT_INT_EQUALS(BACKWARD, move_instr.dir_1);
    ASSERT_INT_EQUALS(1, move_instr.steps_2);
    ASSERT_INT_EQUALS(FORWARD, move_instr.dir_2);
    return true;
}

bool test_move_adjacent_right()
{
    current_pos_x = 10;
    current_pos_y = 10;
    set_target_pos(11, 10);
    ASSERT_FALSE(at_target());
    move_one_instruction_t move_instr;
    move_toward_target(&move_instr);
    ASSERT_TRUE(at_target());
    ASSERT_INT_EQUALS(1, move_instr.steps_1);
    ASSERT_INT_EQUALS(FORWARD, move_instr.dir_1);
    ASSERT_INT_EQUALS(1, move_instr.steps_2);
    ASSERT_INT_EQUALS(FORWARD, move_instr.dir_2);
    return true;
}

bool test_move_adjacent_down()
{
    current_pos_x = 10;
    current_pos_y = 10;
    set_target_pos(10, 9);
    ASSERT_FALSE(at_target());
    move_one_instruction_t move_instr;
    move_toward_target(&move_instr);
    ASSERT_TRUE(at_target());
    ASSERT_INT_EQUALS(1, move_instr.steps_1);
    ASSERT_INT_EQUALS(FORWARD, move_instr.dir_1);
    ASSERT_INT_EQUALS(1, move_instr.steps_2);
    ASSERT_INT_EQUALS(BACKWARD, move_instr.dir_2);
    return true;
}

bool test_move_adjacent_left()
{
    current_pos_x = 10;
    current_pos_y = 10;
    set_target_pos(9, 10);
    ASSERT_FALSE(at_target());
    move_one_instruction_t move_instr;
    move_toward_target(&move_instr);
    ASSERT_TRUE(at_target());
    ASSERT_INT_EQUALS(1, move_instr.steps_1);
    ASSERT_INT_EQUALS(BACKWARD, move_instr.dir_1);
    ASSERT_INT_EQUALS(1, move_instr.steps_2);
    ASSERT_INT_EQUALS(BACKWARD, move_instr.dir_2);
    return true;
}

bool test_move_diag_up_right()
{
    current_pos_x = 10;
    current_pos_y = 10;
    set_target_pos(11, 11);
    ASSERT_FALSE(at_target());
    move_one_instruction_t move_instr;
    move_toward_target(&move_instr);
    ASSERT_TRUE(at_target());
    ASSERT_INT_EQUALS(0, move_instr.steps_1);
    ASSERT_INT_EQUALS(2, move_instr.steps_2);
    ASSERT_INT_EQUALS(FORWARD, move_instr.dir_2);
    return true;
}

bool test_move_diag_down_right()
{
    current_pos_x = 10;
    current_pos_y = 10;
    set_target_pos(11, 9);
    ASSERT_FALSE(at_target());
    move_one_instruction_t move_instr;
    move_toward_target(&move_instr);
    ASSERT_TRUE(at_target());
    ASSERT_INT_EQUALS(2, move_instr.steps_1);
    ASSERT_INT_EQUALS(FORWARD, move_instr.dir_1);
    ASSERT_INT_EQUALS(0, move_instr.steps_2);
    return true;
}

bool test_move_diag_down_left()
{
    current_pos_x = 10;
    current_pos_y = 10;
    set_target_pos(9, 9);
    ASSERT_FALSE(at_target());
    move_one_instruction_t move_instr;
    move_toward_target(&move_instr);
    ASSERT_TRUE(at_target());
    ASSERT_INT_EQUALS(0, move_instr.steps_1);
    ASSERT_INT_EQUALS(2, move_instr.steps_2);
    ASSERT_INT_EQUALS(BACKWARD, move_instr.dir_2);
    return true;
}

bool test_move_diag_up_left()
{
    current_pos_x = 10;
    current_pos_y = 10;
    set_target_pos(9, 11);
    ASSERT_FALSE(at_target());
    move_one_instruction_t move_instr;
    move_toward_target(&move_instr);
    ASSERT_TRUE(at_target());
    ASSERT_INT_EQUALS(2, move_instr.steps_1);
    ASSERT_INT_EQUALS(BACKWARD, move_instr.dir_1);
    ASSERT_INT_EQUALS(0, move_instr.steps_2);
    return true;
}






//***************************************
// Main Unit Test Driver
//***************************************

typedef struct unit_test {
    const char *test_name;
    bool (*test_func_ptr)(void);
} unit_test_t;

unit_test_t test_list[] = {
    {"test_move_adjacent_up    ", test_move_adjacent_up},
    {"test_move_adjacent_right ", test_move_adjacent_right},
    {"test_move_adjacent_down  ", test_move_adjacent_down},
    {"test_move_adjacent_left  ", test_move_adjacent_left},
    {"test_move_diag_up_right  ", test_move_diag_up_right},
    {"test_move_diag_down_right", test_move_diag_down_right},
    {"test_move_diag_down_left ", test_move_diag_down_left},
    {"test_move_diag_up_left   ", test_move_diag_up_left},
    {nullptr, nullptr}
};

int main()
{
    int test_idx = 0;
    int num_failures = 0;
    int total_test_num = 0;
    
    // count total number of unit tests before starting
    while (test_list[total_test_num].test_name) {
        total_test_num++;
    }
    
    std::cout << std::endl;
    
    while (test_list[test_idx].test_name)
    {
        std::cout << "Test [" << (test_idx+1) << "/" << total_test_num << "]: " << test_list[test_idx].test_name << ": ";
        int rc = test_list[test_idx].test_func_ptr();
        if (rc) {
            std::cout << "PASSED" << std::endl;
        } else {
            num_failures++;
        }
        
        test_idx++;
    }
    
    if (num_failures > 0) {
        std::cout << std::endl << num_failures << " out of " << total_test_num << " failed" << std::endl;
        return 1;
    }
    
    std::cout << "All test passed" << std::endl;
    return 0;
}