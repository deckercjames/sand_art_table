// Your First C++ Program

#include <iostream>

#include "path_calculator.h"
#include "config.h"

#define ASSERT_FAILURE do {;} while(0)
#define ASSERT_TRUE(x) if (!x) {std::cout << "FAILED line " << __LINE__ << " expected true, was false" << std::endl; return false;}
#define ASSERT_FALSE(x) if (x) {std::cout << "FAILED line " << __LINE__ << " expected false, was true" << std::endl; return false;}
#define ASSERT_INT_EQUALS(exp, act) if (act != exp) {std::cout << "FAILED line " << __LINE__ << " expected " << \
                                        exp << ", was " << act << std::endl; return false;}

// Redefine the stepper motor directions for implementation
// Don't include the arduino header so we can unit test easier
#define FORWARD 1
#define BACKWARD 2


//***************************************
// Get Movement Tests
//***************************************

bool test_move_adjacent_up()
{
    current_pos_x = 10;
    current_pos_y = 10;
    set_target_pos(10, 11);
    ASSERT_FALSE(at_target());
    move_one_instruction_t move_instr;
    get_motor_movement_instructions(&move_instr);
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
    get_motor_movement_instructions(&move_instr);
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
    get_motor_movement_instructions(&move_instr);
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
    get_motor_movement_instructions(&move_instr);
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
    get_motor_movement_instructions(&move_instr);
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
    get_motor_movement_instructions(&move_instr);
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
    get_motor_movement_instructions(&move_instr);
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
    get_motor_movement_instructions(&move_instr);
    ASSERT_TRUE(at_target());
    ASSERT_INT_EQUALS(2, move_instr.steps_1);
    ASSERT_INT_EQUALS(BACKWARD, move_instr.dir_1);
    ASSERT_INT_EQUALS(0, move_instr.steps_2);
    return true;
}


//***************************************
// Xcode parse Tests
//***************************************


bool test_gcode_basic()
{
    set_target_position_gcode("G1 X10 Y20");
    // Use at_target to check that the target pos was set correctly
    current_pos_x = 100;
    current_pos_y = 200;
    ASSERT_TRUE(at_target());
    return true;
}

bool test_gcode_basic_presision()
{
    set_target_position_gcode("G1 X10.000 Y20.000");
    // Use at_target to check that the target pos was set correctly
    current_pos_x = 100;
    current_pos_y = 200;
    ASSERT_TRUE(at_target());
    return true;
}

bool test_gcode_decimel()
{
    set_target_position_gcode("G1 X10.200 Y30.400");
    // Use at_target to check that the target pos was set correctly
    current_pos_x = 102;
    current_pos_y = 304;
    ASSERT_TRUE(at_target());
    return true;
}

bool test_gcode_subone()
{
    set_target_position_gcode("G1 X0.100 Y0.200");
    // Use at_target to check that the target pos was set correctly
    current_pos_x = 1;
    current_pos_y = 2;
    ASSERT_TRUE(at_target());
    return true;
}

bool test_gcode_subone_no_leading_zero()
{
    set_target_position_gcode("G1 X.100 Y.200");
    // Use at_target to check that the target pos was set correctly
    current_pos_x = 1;
    current_pos_y = 2;
    ASSERT_TRUE(at_target());
    return true;
}

bool test_gcode_decimel_truncation()
{
    set_target_position_gcode("G1 X12.345 Y45.678");
    // Use at_target to check that the target pos was set correctly
    current_pos_x = 123;
    current_pos_y = 456;
    ASSERT_TRUE(at_target());
    return true;
}

bool test_gcode_decimel_no_presision()
{
    set_target_position_gcode("G1 X12. Y34.");
    // Use at_target to check that the target pos was set correctly
    current_pos_x = 120;
    current_pos_y = 340;
    ASSERT_TRUE(at_target());
    return true;
}

bool test_gcode_truncate_to_zero()
{
    set_target_position_gcode("G1 X0.090 Y0.080");
    // Use at_target to check that the target pos was set correctly
    current_pos_x = 0;
    current_pos_y = 0;
    ASSERT_TRUE(at_target());
    return true;
}

bool test_gcode_zero_padded_g()
{
    set_target_position_gcode("G01 X10.000 Y20.000");
    // Use at_target to check that the target pos was set correctly
    current_pos_x = 100;
    current_pos_y = 200;
    ASSERT_TRUE(at_target());
    return true;
}

bool test_gcode_zero_padded_g_many()
{
    set_target_position_gcode("G0001 X10.000 Y20.000");
    // Use at_target to check that the target pos was set correctly
    current_pos_x = 100;
    current_pos_y = 200;
    ASSERT_TRUE(at_target());
    return true;
}

bool test_gcode_basic_comment()
{
    set_target_position_gcode("G1 X10 Y20; Move to this position");
    // Use at_target to check that the target pos was set correctly
    current_pos_x = 100;
    current_pos_y = 200;
    ASSERT_TRUE(at_target());
    return true;
}

bool test_gcode_all_comment()
{
    current_pos_x = 100;
    current_pos_y = 200;
    set_target_pos(100, 200);
    ASSERT_TRUE(at_target());
    // No-op instruction should not change that it is at the target position
    set_target_position_gcode("; Move to this position");
    ASSERT_TRUE(at_target());
    return true;
}

bool test_gcode_empty_line()
{
    current_pos_x = 100;
    current_pos_y = 200;
    set_target_pos(100, 200);
    ASSERT_TRUE(at_target());
    // No-op instruction should not change that it is at the target position
    set_target_position_gcode("");
    ASSERT_TRUE(at_target());
    return true;
}

bool test_gcode_invalid()
{
    current_pos_x = 100;
    current_pos_y = 200;
    set_target_pos(100, 200);
    ASSERT_TRUE(at_target());
    // No-op instruction should not change that it is at the target position
    set_target_position_gcode("invalid");
    ASSERT_TRUE(at_target());
    return true;
}

bool test_gcode_non_g1_command()
{
    current_pos_x = 100;
    current_pos_y = 200;
    set_target_pos(100, 200);
    ASSERT_TRUE(at_target());
    // Anything other than a g1 command should be considered a no op
    set_target_position_gcode("G2 X30 Y40");
    ASSERT_TRUE(at_target());
    return true;
}

bool test_gcode_extra_args()
{
    // The "Extrude" command should be ignored
    set_target_position_gcode("G1 X10 Y20 E30");
    // Use at_target to check that the target pos was set correctly
    current_pos_x = 100;
    current_pos_y = 200;
    ASSERT_TRUE(at_target());
    return true;
}

bool test_gcode_x_arg_only()
{
    current_pos_x = 100;
    current_pos_y = 200;
    set_target_pos(100, 200);
    ASSERT_TRUE(at_target());
    // The Y target position should stay the same
    set_target_position_gcode("G2 X30");
    ASSERT_FALSE(at_target());
    current_pos_x = 300;
    current_pos_y = 200;
    ASSERT_TRUE(at_target());
    return true;
}

bool test_gcode_y_arg_only()
{
    current_pos_x = 100;
    current_pos_y = 200;
    set_target_pos(100, 200);
    ASSERT_TRUE(at_target());
    // The Y target position should stay the same
    set_target_position_gcode("G2 Y30");
    ASSERT_FALSE(at_target());
    current_pos_x = 100;
    current_pos_y = 300;
    ASSERT_TRUE(at_target());
    return true;
}

bool test_gcode_neg_x()
{
    set_target_position_gcode("G1 X-10 Y20");
    // Use at_target to check that the target pos was set correctly
    current_pos_x = 0;
    current_pos_y = 200;
    ASSERT_TRUE(at_target());
    return true;
}

bool test_gcode_neg_y()
{
    set_target_position_gcode("G1 X10 Y-20");
    // Use at_target to check that the target pos was set correctly
    current_pos_x = 100;
    current_pos_y = 0;
    ASSERT_TRUE(at_target());
    return true;
}

bool test_gcode_overflow_x()
{
    set_target_position_gcode("G1 X10000000000 Y20");
    // Use at_target to check that the target pos was set correctly
    current_pos_x = TABLE_DIM_X_MM;
    current_pos_y = 200;
    ASSERT_TRUE(at_target());
    return true;
}

bool test_gcode_overflow_y()
{
    set_target_position_gcode("G1 X10 Y20000000000000");
    // Use at_target to check that the target pos was set correctly
    current_pos_x = 100;
    current_pos_y = TABLE_DIM_Y_MM;
    ASSERT_TRUE(at_target());
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
    
    {"test_gcode_basic                 ", test_gcode_basic},
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
    {"test_gcode_invalid               ", test_gcode_invalid},
    {"test_gcode_non_g1_command        ", test_gcode_non_g1_command},
    {"test_gcode_extra_args            ", test_gcode_extra_args},
    {"test_gcode_x_arg_only            ", test_gcode_x_arg_only},
    {"test_gcode_y_arg_only            ", test_gcode_y_arg_only},
    {"test_gcode_neg_x                 ", test_gcode_neg_x},
    {"test_gcode_neg_y                 ", test_gcode_neg_y},
    {"test_gcode_overflow_x            ", test_gcode_overflow_x},
    {"test_gcode_overflow_y            ", test_gcode_overflow_y},
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