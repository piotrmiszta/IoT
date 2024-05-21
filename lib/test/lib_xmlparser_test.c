//
// Created by piotr on 06.05.24.
//
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include "../src/xml_parser.c"

typedef struct test_data
{
    char* test_buff;
    char* file_buff;
}test_data;

static int initialize(void** state)
{
    test_data* test = allocator_alloc(sizeof(test_data));
    char* parse = xml_read("../../../lib/test/files/simple.xml");
    char temp[] = "<device><interfaces><test>Testing data</test></interfaces><protocols>\n</protocols>\n</device>\n";
    char* test_buff = allocator_alloc(sizeof(temp));
    memcpy(test_buff, temp, sizeof(temp));
    test->file_buff = parse;
    test->test_buff = test_buff;
    *state = test;

    return 0;
}

static int teardown(void** state)
{
    test_data* data = *state;
    test_free(data->test_buff);
    allocator_free(data->file_buff);
    test_free(data);
    return 0;
}

static void test_read(void** state)
{
    test_data* data = *state;
    assert_string_equal(data->file_buff, data->test_buff);
}

static void get_begin_end_tag(void** state)
{
    //               00000000001111111111222222222233333333334444
    //               01234567890123456789012345678901234567890123
    char buffer[] = "<device>   <test>      </test>     </device>";
    const char* b1 = get_begin_of_tag(buffer);
    const char* e1 = get_end_of_tag(b1);
    assert_ptr_equal(b1, buffer);
    assert_ptr_equal(e1, buffer + 7);
    const char* b2 = get_begin_of_tag(e1);
    const char* e2 = get_end_of_tag(b2);
    assert_ptr_equal(b2, buffer + 11);
    assert_ptr_equal(e2, buffer + 16);
    const char* b3 = get_begin_of_tag(e2);
    const char* e3 = get_end_of_tag(b3);
    assert_ptr_equal(b3, buffer + 23);
    assert_ptr_equal(e3, buffer + 29);
    const char* b4 = get_begin_of_tag(e3);
    const char* e4 = get_end_of_tag(b4);
    assert_ptr_equal(b4, buffer + 35);
    assert_ptr_equal(e4, buffer + 43);
}

static void get_open_close_tag(void** state)
{
    //               00000000001111111111222222222233333333334444
    //               01234567890123456789012345678901234567890123
    char buffer[] = "<device>   <test>      </test>     </device>";
    const char* o1 = get_open_tag(buffer);
    const char* o2 = get_open_tag(get_end_of_tag(o1));
    const char* c1 = get_close_tag(buffer);
    const char* c2 = get_close_tag(get_end_of_tag(c1));
    assert_ptr_equal(o1, buffer + 0);
    assert_ptr_equal(o2, buffer + 11);
    assert_ptr_equal(c1, buffer + 23);
    assert_ptr_equal(c2, buffer + 35);
}

static void get_name_of_tag_test(void** state)
{
    //             000000000011111111112222222222
    //             012345678901234567890123456789
    char buff[] = "<device><test></test></device>";
    xml_string s1 = get_tag_name(buff);
    const char* b = get_begin_of_tag(buff);
    b = get_end_of_tag(b);
    xml_string s2 = get_tag_name(b);
    b = get_begin_of_tag(b);
    b = get_end_of_tag(b);
    xml_string s3 = get_tag_name(b);
    b = get_begin_of_tag(b);
    b = get_end_of_tag(b);
    xml_string s4 = get_tag_name(b);

    char* str1 = xml_string_to_char(s1);
    char* str2 = xml_string_to_char(s2);
    char* str3 = xml_string_to_char(s3);
    char* str4 = xml_string_to_char(s4);

    assert_string_equal(str1, "device");
    assert_string_equal(str2, "test");
    assert_string_equal(str3, "test");
    assert_string_equal(str4, "device");
    allocator_free(str1);
    allocator_free(str2);
    allocator_free(str3);
    allocator_free(str4);

}

static void get_end_tag_by_name_test(void** state)
{
    //             000000000011111111112222222222
    //             012345678901234567890123456789
    char buff[] = "<device><test></test></device>";
    xml_string s1 = get_tag_name(buff);
    char* str1 = xml_string_to_char(s1);
    assert_string_equal(str1, "device");
    const char* ptr = get_close_tag_by_name(buff, s1);
    assert_ptr_equal(ptr, buff + 21);
    allocator_free(str1);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_read),
        cmocka_unit_test(get_begin_end_tag),
        cmocka_unit_test(get_open_close_tag),
        cmocka_unit_test(get_name_of_tag_test),
        cmocka_unit_test(get_end_tag_by_name_test),
};
    return cmocka_run_group_tests_name("queue_success_tests",tests, initialize, teardown);
}