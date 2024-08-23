
#include "unit_test_utils.h"

#include "gcode_parser.h"


bool test_gcode_basic()
{
    location_msg_t result = { 0 };
    parse_gcode_line("G1 X10 Y20", &result);
    ASSERT_INT_EQUALS(100, result.x_location_steps);
    ASSERT_INT_EQUALS(200, result.y_location_steps);
    return true;
}

bool test_gcode_args_reversed()
{
    location_msg_t result = { 0 };
    parse_gcode_line("G1 Y20 X10", &result);
    ASSERT_INT_EQUALS(100, result.x_location_steps);
    ASSERT_INT_EQUALS(200, result.y_location_steps);
    return true;
}

bool test_gcode_basic_presision()
{
    location_msg_t result = { 0 };
    parse_gcode_line("G1 X10.000 Y20.000", &result);
    ASSERT_INT_EQUALS(100, result.x_location_steps);
    ASSERT_INT_EQUALS(200, result.y_location_steps);
    return true;
}

bool test_gcode_decimel()
{
    location_msg_t result = { 0 };
    parse_gcode_line("G1 X10.200 Y30.400", &result);
    ASSERT_INT_EQUALS(102, result.x_location_steps);
    ASSERT_INT_EQUALS(304, result.y_location_steps);
    return true;
}

bool test_gcode_subone()
{
    location_msg_t result = { 0 };
    parse_gcode_line("G1 X0.100 Y0.200", &result);
    ASSERT_INT_EQUALS(1, result.x_location_steps);
    ASSERT_INT_EQUALS(2, result.y_location_steps);
    return true;
}

bool test_gcode_subone_no_leading_zero()
{
    location_msg_t result = { 0 };
    parse_gcode_line("G1 X.100 Y.200", &result);
    ASSERT_INT_EQUALS(1, result.x_location_steps);
    ASSERT_INT_EQUALS(2, result.y_location_steps);
    return true;
}

bool test_gcode_decimel_truncation()
{
    location_msg_t result = { 0 };
    parse_gcode_line("G1 X12.345 Y45.678", &result);
    ASSERT_INT_EQUALS(123, result.x_location_steps);
    ASSERT_INT_EQUALS(456, result.y_location_steps);
    return true;
}

bool test_gcode_decimel_no_presision()
{
    location_msg_t result = { 0 };
    parse_gcode_line("G1 X12. Y34.", &result);
    ASSERT_INT_EQUALS(120, result.x_location_steps);
    ASSERT_INT_EQUALS(340, result.y_location_steps);
    return true;
}

bool test_gcode_truncate_to_zero()
{
    location_msg_t result = { 0 };
    parse_gcode_line("G1 X0.090 Y0.080", &result);
    ASSERT_INT_EQUALS(0, result.x_location_steps);
    ASSERT_INT_EQUALS(0, result.y_location_steps);
    return true;
}

bool test_gcode_zero_padded_g()
{
    location_msg_t result = { 0 };
    parse_gcode_line("G01 X10.000 Y20.000", &result);
    ASSERT_INT_EQUALS(100, result.x_location_steps);
    ASSERT_INT_EQUALS(200, result.y_location_steps);
    return true;
}

bool test_gcode_zero_padded_g_many()
{
    location_msg_t result = { 0 };
    parse_gcode_line("G0001 X10.000 Y20.000", &result);
    ASSERT_INT_EQUALS(100, result.x_location_steps);
    ASSERT_INT_EQUALS(200, result.y_location_steps);
    return true;
}

bool test_gcode_basic_comment()
{
    location_msg_t result = { 0 };
    parse_gcode_line("G1 X10 Y20; Move to this position", &result);
    ASSERT_INT_EQUALS(100, result.x_location_steps);
    ASSERT_INT_EQUALS(200, result.y_location_steps);
    return true;
}

bool test_gcode_comment_after_extrude()
{
    location_msg_t result = { 0 };
    parse_gcode_line("G1 X10 Y20 E30; Move to this position", &result);
    ASSERT_INT_EQUALS(100, result.x_location_steps);
    ASSERT_INT_EQUALS(200, result.y_location_steps);
    return true;
}

bool test_gcode_trailing_separated_comment()
{
    location_msg_t result = { 0 };
    parse_gcode_line("G1 X10 Y20 ; Move to this position", &result);
    ASSERT_INT_EQUALS(100, result.x_location_steps);
    ASSERT_INT_EQUALS(200, result.y_location_steps);
    return true;
}

bool test_gcode_g_comment()
{
    location_msg_t result = { 
        .x_location_steps = 100,
        .y_location_steps = 200,
    };
    parse_gcode_line("G; Move to this position", &result);
    ASSERT_INT_EQUALS(100, result.x_location_steps);
    ASSERT_INT_EQUALS(200, result.y_location_steps);
    return true;
}

bool test_gcode_g0_comment()
{
    location_msg_t result = { 
        .x_location_steps = 100,
        .y_location_steps = 200,
    };
    parse_gcode_line("G0; Move to this position", &result);
    ASSERT_INT_EQUALS(100, result.x_location_steps);
    ASSERT_INT_EQUALS(200, result.y_location_steps);
    return true;
}

bool test_gcode_g01_comment()
{
    location_msg_t result = { 
        .x_location_steps = 100,
        .y_location_steps = 200,
    };
    parse_gcode_line("G01; Move to this position", &result);
    ASSERT_INT_EQUALS(100, result.x_location_steps);
    ASSERT_INT_EQUALS(200, result.y_location_steps);
    return true;
}

bool test_gcode_all_comment()
{
    location_msg_t result = { 
        .x_location_steps = 100,
        .y_location_steps = 200,
    };
    parse_gcode_line("; Move to this position", &result);
    ASSERT_INT_EQUALS(100, result.x_location_steps);
    ASSERT_INT_EQUALS(200, result.y_location_steps);
    return true;
}

bool test_gcode_empty_line()
{
    location_msg_t result = { 
        .x_location_steps = 100,
        .y_location_steps = 200,
    };
    parse_gcode_line("", &result);
    ASSERT_INT_EQUALS(100, result.x_location_steps);
    ASSERT_INT_EQUALS(200, result.y_location_steps);
    return true;
}

bool test_gcode_ginstruction_only()
{
    location_msg_t result = { 
        .x_location_steps = 100,
        .y_location_steps = 200,
    };
    parse_gcode_line("G01", &result);
    ASSERT_INT_EQUALS(100, result.x_location_steps);
    ASSERT_INT_EQUALS(200, result.y_location_steps);
    return true;
}

bool test_gcode_invalid()
{
    location_msg_t result = { 
        .x_location_steps = 100,
        .y_location_steps = 200,
    };
    parse_gcode_line("invalid", &result);
    ASSERT_INT_EQUALS(100, result.x_location_steps);
    ASSERT_INT_EQUALS(200, result.y_location_steps);
    return true;
}

bool test_gcode_non_g1_command()
{
    location_msg_t result = { 
        .x_location_steps = 100,
        .y_location_steps = 200,
    };
    parse_gcode_line("G2 X30 Y40", &result);
    ASSERT_INT_EQUALS(100, result.x_location_steps);
    ASSERT_INT_EQUALS(200, result.y_location_steps);
    return true;
}

bool test_gcode_extra_arg()
{
    location_msg_t result = { 0 };
    // The "Extrude" command should be ignored
    parse_gcode_line("G1 X10 Y20 E30", &result);
    ASSERT_INT_EQUALS(100, result.x_location_steps);
    ASSERT_INT_EQUALS(200, result.y_location_steps);
    return true;
}

bool test_gcode_extra_arg_middle()
{
    location_msg_t result = { 0 };
    // The "Extrude" command should be ignored
    parse_gcode_line("G1 X10 E30 Y20", &result);
    ASSERT_INT_EQUALS(100, result.x_location_steps);
    ASSERT_INT_EQUALS(200, result.y_location_steps);
    return true;
}

bool test_gcode_x_arg_only()
{
    location_msg_t result = { 
        .x_location_steps = 100,
        .y_location_steps = 200,
    };
    parse_gcode_line("G1 X30", &result);
    ASSERT_INT_EQUALS(300, result.x_location_steps);
    ASSERT_INT_EQUALS(200, result.y_location_steps);
    return true;
}

bool test_gcode_y_arg_only()
{
    location_msg_t result = { 
        .x_location_steps = 100,
        .y_location_steps = 200,
    };
    parse_gcode_line("G1 Y30", &result);
    ASSERT_INT_EQUALS(100, result.x_location_steps);
    ASSERT_INT_EQUALS(300, result.y_location_steps);
    return true;
}

bool test_gcode_neg_x()
{
    location_msg_t result = { 0 };
    parse_gcode_line("G1 X-10 Y20", &result);
    ASSERT_INT_EQUALS(0, result.x_location_steps);
    ASSERT_INT_EQUALS(200, result.y_location_steps);
    return true;
}

bool test_gcode_neg_y()
{
    location_msg_t result = { 0 };
    parse_gcode_line("G1 X10 Y-20", &result);
    ASSERT_INT_EQUALS(100, result.x_location_steps);
    ASSERT_INT_EQUALS(0, result.y_location_steps);
    return true;
}

bool test_gcode_overflow_x()
{
    location_msg_t result = { 
        .x_location_steps = 100,
        .y_location_steps = 200,
    };
    parse_gcode_line("G1 X100000000000000000000 Y30", &result);
    // Check that target position did not get updated for the offending
    ASSERT_INT_EQUALS(100, result.x_location_steps);
    ASSERT_INT_EQUALS(300, result.y_location_steps);
    return true;
}

bool test_gcode_overflow_y()
{
    location_msg_t result = { 
        .x_location_steps = 100,
        .y_location_steps = 200,
    };
    parse_gcode_line("G1 X30 Y100000000000000000000", &result);
    // Check that target position did not get updated for the offending
    ASSERT_INT_EQUALS(300, result.x_location_steps);
    ASSERT_INT_EQUALS(200, result.y_location_steps);
    return true;
}

bool test_gcode_leading_spaces()
{
    location_msg_t result = { 0 };
    parse_gcode_line("  G1 X10 Y20", &result);
    ASSERT_INT_EQUALS(100, result.x_location_steps);
    ASSERT_INT_EQUALS(200, result.y_location_steps);
    return true;
}

bool test_gcode_multiple_spaces()
{
    location_msg_t result = { 0 };
    parse_gcode_line("G1  X10       Y20     ", &result);
    ASSERT_INT_EQUALS(100, result.x_location_steps);
    ASSERT_INT_EQUALS(200, result.y_location_steps);
    return true;
}


unit_test_t test_list[] = {
    TEST_CASE(test_gcode_basic),
    TEST_CASE(test_gcode_args_reversed),
    TEST_CASE(test_gcode_basic_presision),
    TEST_CASE(test_gcode_decimel),
    TEST_CASE(test_gcode_subone),
    TEST_CASE(test_gcode_subone_no_leading_zero),
    TEST_CASE(test_gcode_decimel_truncation),
    TEST_CASE(test_gcode_decimel_no_presision),
    TEST_CASE(test_gcode_truncate_to_zero),
    TEST_CASE(test_gcode_zero_padded_g),
    TEST_CASE(test_gcode_zero_padded_g_many),
    TEST_CASE(test_gcode_basic_comment),
    TEST_CASE(test_gcode_g_comment),
    TEST_CASE(test_gcode_g0_comment),
    TEST_CASE(test_gcode_g01_comment),
    TEST_CASE(test_gcode_all_comment),
    TEST_CASE(test_gcode_empty_line),
    TEST_CASE(test_gcode_ginstruction_only),
    TEST_CASE(test_gcode_invalid),
    TEST_CASE(test_gcode_non_g1_command),
    TEST_CASE(test_gcode_extra_arg),
    TEST_CASE(test_gcode_extra_arg_middle),
    TEST_CASE(test_gcode_x_arg_only),
    TEST_CASE(test_gcode_y_arg_only),
    TEST_CASE(test_gcode_neg_x),
    TEST_CASE(test_gcode_neg_y),
    TEST_CASE(test_gcode_overflow_x),
    TEST_CASE(test_gcode_overflow_y),
    TEST_CASE(test_gcode_leading_spaces),
    TEST_CASE(test_gcode_multiple_spaces),
    
    {nullptr, nullptr}
};

MAIN(test_list);