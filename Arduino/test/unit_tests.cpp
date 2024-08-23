// Your First C++ Program

#include <iostream>

#include "path_calculator.h"
#include "config.h"

#define ASSERT_TRUE(x) if (!x) {std::cout << "FAILED line " << __LINE__ << " expected true, was false" << std::endl; return false;}
#define ASSERT_FALSE(x) if (x) {std::cout << "FAILED line " << __LINE__ << " expected false, was true" << std::endl; return false;}
#define ASSERT_INT_EQUALS(exp, act) if (act != exp) {std::cout << "FAILED line " << __LINE__ << " expected " << \
                                        exp << ", was " << act << std::endl; return false;}

// Redefine the stepper motor directions for implementation
// Don't include the arduino header so we can unit test easier
#define FORWARD 1
#define BACKWARD 2

#define UM100_TO_STEPS(x) ((x) / 2)
#define TEST_MM_TO_STEPS(x) ((x) * 5)

//***************************************
// Get Movement Tests
//***************************************

bool test_move_adjacent_up()
{
    current_pos_x = 10;
    current_pos_y = 10;
    set_target_pos_steps(10, 11);
    ASSERT_FALSE(at_target());
    move_one_instruction_t move_instr_t;
    get_motor_movement_instructions(&move_instr_t);
    ASSERT_TRUE(at_target());
    ASSERT_INT_EQUALS(1, move_instr_t.steps_1);
    ASSERT_INT_EQUALS(BACKWARD, move_instr_t.dir_1);
    ASSERT_INT_EQUALS(1, move_instr_t.steps_2);
    ASSERT_INT_EQUALS(FORWARD, move_instr_t.dir_2);
    return true;
}

bool test_move_adjacent_right()
{
    current_pos_x = 10;
    current_pos_y = 10;
    set_target_pos_steps(11, 10);
    ASSERT_FALSE(at_target());
    move_one_instruction_t move_instr_t;
    get_motor_movement_instructions(&move_instr_t);
    ASSERT_TRUE(at_target());
    ASSERT_INT_EQUALS(1, move_instr_t.steps_1);
    ASSERT_INT_EQUALS(FORWARD, move_instr_t.dir_1);
    ASSERT_INT_EQUALS(1, move_instr_t.steps_2);
    ASSERT_INT_EQUALS(FORWARD, move_instr_t.dir_2);
    return true;
}

bool test_move_adjacent_down()
{
    current_pos_x = 10;
    current_pos_y = 10;
    set_target_pos_steps(10, 9);
    ASSERT_FALSE(at_target());
    move_one_instruction_t move_instr_t;
    get_motor_movement_instructions(&move_instr_t);
    ASSERT_TRUE(at_target());
    ASSERT_INT_EQUALS(1, move_instr_t.steps_1);
    ASSERT_INT_EQUALS(FORWARD, move_instr_t.dir_1);
    ASSERT_INT_EQUALS(1, move_instr_t.steps_2);
    ASSERT_INT_EQUALS(BACKWARD, move_instr_t.dir_2);
    return true;
}

bool test_move_adjacent_left()
{
    current_pos_x = 10;
    current_pos_y = 10;
    set_target_pos_steps(9, 10);
    ASSERT_FALSE(at_target());
    move_one_instruction_t move_instr_t;
    get_motor_movement_instructions(&move_instr_t);
    ASSERT_TRUE(at_target());
    ASSERT_INT_EQUALS(1, move_instr_t.steps_1);
    ASSERT_INT_EQUALS(BACKWARD, move_instr_t.dir_1);
    ASSERT_INT_EQUALS(1, move_instr_t.steps_2);
    ASSERT_INT_EQUALS(BACKWARD, move_instr_t.dir_2);
    return true;
}

bool test_move_diag_up_right()
{
    current_pos_x = 10;
    current_pos_y = 10;
    set_target_pos_steps(11, 11);
    ASSERT_FALSE(at_target());
    move_one_instruction_t move_instr_t;
    get_motor_movement_instructions(&move_instr_t);
    ASSERT_TRUE(at_target());
    ASSERT_INT_EQUALS(0, move_instr_t.steps_1);
    ASSERT_INT_EQUALS(2, move_instr_t.steps_2);
    ASSERT_INT_EQUALS(FORWARD, move_instr_t.dir_2);
    return true;
}

bool test_move_diag_down_right()
{
    current_pos_x = 10;
    current_pos_y = 10;
    set_target_pos_steps(11, 9);
    ASSERT_FALSE(at_target());
    move_one_instruction_t move_instr_t;
    get_motor_movement_instructions(&move_instr_t);
    ASSERT_TRUE(at_target());
    ASSERT_INT_EQUALS(2, move_instr_t.steps_1);
    ASSERT_INT_EQUALS(FORWARD, move_instr_t.dir_1);
    ASSERT_INT_EQUALS(0, move_instr_t.steps_2);
    return true;
}

bool test_move_diag_down_left()
{
    current_pos_x = 10;
    current_pos_y = 10;
    set_target_pos_steps(9, 9);
    ASSERT_FALSE(at_target());
    move_one_instruction_t move_instr_t;
    get_motor_movement_instructions(&move_instr_t);
    ASSERT_TRUE(at_target());
    ASSERT_INT_EQUALS(0, move_instr_t.steps_1);
    ASSERT_INT_EQUALS(2, move_instr_t.steps_2);
    ASSERT_INT_EQUALS(BACKWARD, move_instr_t.dir_2);
    return true;
}

bool test_move_diag_up_left()
{
    current_pos_x = 10;
    current_pos_y = 10;
    set_target_pos_steps(9, 11);
    ASSERT_FALSE(at_target());
    move_one_instruction_t move_instr_t;
    get_motor_movement_instructions(&move_instr_t);
    ASSERT_TRUE(at_target());
    ASSERT_INT_EQUALS(2, move_instr_t.steps_1);
    ASSERT_INT_EQUALS(BACKWARD, move_instr_t.dir_1);
    ASSERT_INT_EQUALS(0, move_instr_t.steps_2);
    return true;
}

bool test_move_twice()
{
    current_pos_x = 10;
    current_pos_y = 10;
    set_target_pos_steps(9, 12);
    ASSERT_FALSE(at_target());
    move_one_instruction_t move_instr_t;
    get_motor_movement_instructions(&move_instr_t);
    ASSERT_FALSE(at_target());
    get_motor_movement_instructions(&move_instr_t);
    ASSERT_TRUE(at_target());
    return true;
}


//***************************************
// Gcode parse Tests
//***************************************

//***************************************
// Gcode parse and move tests
//***************************************

bool test_gcode_and_move()
{
    set_target_position_gcode("G1 X10.2 Y20");
    // Use at_target to check that the target pos was set correctly
    current_pos_x = UM100_TO_STEPS(100);
    current_pos_y = UM100_TO_STEPS(200);
    ASSERT_FALSE(at_target());
    move_one_instruction_t move_instr_t;
    get_motor_movement_instructions(&move_instr_t);
    ASSERT_TRUE(at_target());
    ASSERT_INT_EQUALS(1, move_instr_t.steps_1);
    ASSERT_INT_EQUALS(FORWARD, move_instr_t.dir_1);
    ASSERT_INT_EQUALS(1, move_instr_t.steps_2);
    ASSERT_INT_EQUALS(FORWARD, move_instr_t.dir_2);
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
    {"test_move_adjacent_up            ", test_move_adjacent_up},
    {"test_move_adjacent_right         ", test_move_adjacent_right},
    {"test_move_adjacent_down          ", test_move_adjacent_down},
    {"test_move_adjacent_left          ", test_move_adjacent_left},
    {"test_move_diag_up_right          ", test_move_diag_up_right},
    {"test_move_diag_down_right        ", test_move_diag_down_right},
    {"test_move_diag_down_left         ", test_move_diag_down_left},
    {"test_move_diag_up_left           ", test_move_diag_up_left},
    {"test_move_twice                  ", test_move_twice},
    
    {"test_gcode_basic                 ", test_gcode_basic},
    {"test_gcode_args_reversed         ", test_gcode_args_reversed},
    {"test_gcode_basic_presision       ", test_gcode_basic_presision},
    {"test_gcode_decimel               ", test_gcode_decimel},
    {"test_gcode_subone                ", test_gcode_subone},
    {"test_gcode_subone_no_leading_zero", test_gcode_subone_no_leading_zero},
    {"test_gcode_decimel_truncation    ", test_gcode_decimel_truncation},
    {"test_gcode_decimel_no_presision  ", test_gcode_decimel_no_presision},
    {"test_gcode_truncate_to_zero      ", test_gcode_truncate_to_zero},
    {"test_gcode_zero_padded_g         ", test_gcode_zero_padded_g},
    {"test_gcode_zero_padded_g_many    ", test_gcode_zero_padded_g_many},
    {"test_gcode_basic_comment         ", test_gcode_basic_comment},
    {"test_gcode_all_comment           ", test_gcode_all_comment},
    {"test_gcode_empty_line            ", test_gcode_empty_line},
    {"test_gcode_ginstruction_only     ", test_gcode_ginstruction_only},
    {"test_gcode_invalid               ", test_gcode_invalid},
    {"test_gcode_non_g1_command        ", test_gcode_non_g1_command},
    {"test_gcode_extra_arg             ", test_gcode_extra_arg},
    {"test_gcode_extra_arg_middle      ", test_gcode_extra_arg_middle},
    {"test_gcode_x_arg_only            ", test_gcode_x_arg_only},
    {"test_gcode_y_arg_only            ", test_gcode_y_arg_only},
    {"test_gcode_neg_x                 ", test_gcode_neg_x},
    {"test_gcode_neg_y                 ", test_gcode_neg_y},
    {"test_gcode_overflow_x            ", test_gcode_overflow_x},
    {"test_gcode_overflow_y            ", test_gcode_overflow_y},
    {"test_gcode_x_too_large           ", test_gcode_x_too_large},
    {"test_gcode_y_too_large           ", test_gcode_y_too_large},
    {"test_gcode_leading_spaces        ", test_gcode_leading_spaces},
    {"test_gcode_multiple_spaces       ", test_gcode_multiple_spaces},
    
    {"test_gcode_and_move              ", test_gcode_and_move},
    {nullptr, nullptr}
};
