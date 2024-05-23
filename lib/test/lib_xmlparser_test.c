//
// Created by piotr on 06.05.24.
//
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include "../src/xml_parser.c"


#ifdef unix
#include "unistd.h"
/* from IOT folder*/
char* get_path(char* relative_path)
{
    char* ret;
    char cwd[100];
    if(getcwd(cwd, sizeof(cwd)) != NULL)
    {
        /* find build folder from end of this cwd */
        char* temp = cwd;
        char* build = temp;
        do
        {
            build = temp;
            temp = strstr(build + 1, "/build");
        }while(temp);
        u32 path_len_to_iot = (u32)(build - cwd);
        u32 size = strlen(relative_path) + path_len_to_iot + 1;
        ret = allocator_calloc(size, 1);
        assert_ptr_not_equal(ret, NULL);
        memcpy(ret, cwd, path_len_to_iot);
        memcpy(&ret[path_len_to_iot], relative_path, strlen(relative_path));

    }
    return ret;
}

#endif

static char* path_to_test_xml;
static char* path_to_simple_xml;

typedef struct test_data
{
    char* test_buff;
    char* file_buff;
}test_data;

static inline void assert_data_name(btree_node_t* node, const char* name, const char* data);

static int initialize(void** state)
{
    path_to_test_xml = get_path("/lib/test/files/test.xml");
    path_to_simple_xml = get_path("/lib/test/files/simple.xml");
    test_data* test = allocator_alloc(sizeof(test_data));
    char* parse = xml_read(path_to_simple_xml);
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
    test_free(path_to_simple_xml);
    test_free(path_to_test_xml);
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

static void parse_test(void** state)
{
    //             000000000011111111112222222222
    //             012345678901234567890123456789
    char buff[] = "<device><test></test></device>";
    xml_parser* parser = allocator_alloc(sizeof(xml_parser));
    parser->filename = NULL;
    parser->tree = btree_create(allocator_free);
    parser->file = test_malloc(sizeof(buff));
    memcpy(parser->file, buff, sizeof(buff));

    xml_parse(parser, buff);
    btree_node_t* tree_node = btree_get_root(parser->tree);
    btree_node_t* dev = btree_get_child(tree_node);
    xml_node* node = btree_get_data(dev);
    char* dev_name = xml_string_to_char(node->name);

    btree_node_t* tes = btree_get_child(dev);
    xml_node* node2 = btree_get_data(tes);
    char* tes_name = xml_string_to_char(node2->name);

    assert_string_equal(tes_name, "test");
    assert_string_equal(dev_name, "device");

    test_free(dev_name);
    test_free(tes_name);
    xml_destroy(parser);
}

static void file_parse_test(void** state)
{
    xml_parser* parser = xml_create(path_to_test_xml);
    btree_node_t* tree_node = btree_get_root(parser->tree);

    btree_node_t* device = btree_get_child(tree_node);
    btree_node_t* interfaces = btree_get_child(device);
    btree_node_t* interface = btree_get_child(interfaces);
    btree_node_t* name = btree_get_child(interface);
    btree_node_t* connected_device = btree_get_sibling(name);
    btree_node_t* baudrate = btree_get_sibling(connected_device);
    btree_node_t* crc = btree_get_sibling(baudrate);
    btree_node_t* parity = btree_get_sibling(crc);
    btree_node_t* null = btree_get_sibling(parity);

    btree_node_t* interface2 = btree_get_sibling(interface);
    btree_node_t* name2 = btree_get_child(interface2);
    btree_node_t* connected_device2 = btree_get_sibling(name2);

    btree_node_t* interface3 = btree_get_sibling(interface2);
    btree_node_t* name3 = btree_get_child(interface3);
    btree_node_t* connected_device3 = btree_get_sibling(name3);

    btree_node_t* protocols = btree_get_sibling(interfaces);
    btree_node_t* protocol = btree_get_child(protocols);
    btree_node_t* name4 = btree_get_child(protocol);
    btree_node_t* ip = btree_get_sibling(name4);
    btree_node_t* port = btree_get_sibling(ip);

    btree_node_t* null2 = btree_get_sibling(port);
    btree_node_t* null3 = btree_get_child(name4);
    btree_node_t* null4 = btree_get_sibling(protocols);

    assert_ptr_equal(null, NULL);
    assert_ptr_equal(null2, NULL);
    assert_ptr_equal(null3, NULL);
    assert_ptr_equal(null4, NULL);

    assert_data_name(device, "device", "");
    assert_data_name(interfaces, "interfaces", "");
    assert_data_name(interface, "interface", "");
    assert_data_name(name, "name", "USARTACM0");
    assert_data_name(connected_device, "connected_device", "STM32H755");
    assert_data_name(baudrate, "baudrate", "115200");
    assert_data_name(crc, "crc", "ENABLED");
    assert_data_name(parity, "parity", "ODD");

    assert_data_name(interface2, "interface", "");
    assert_data_name(name2, "name", "ETH");
    assert_data_name(connected_device2, "connected_device", "NONE" );

    assert_data_name(interface3, "interface", "");
    assert_data_name(name3, "name", "WIFI");
    assert_data_name(connected_device3, "connected_device", "NONE");

    assert_data_name(protocols, "protocols", "");
    assert_data_name(protocol, "protocol", "");
    assert_data_name(name4, "name", "TCP/IP");
    assert_data_name(ip, "ip", "127.0.0.1");
    assert_data_name(port, "port", "8080");

    xml_destroy(parser);
}

static void parse_data_test(void** state)
{
    //             000000000011111111112222222222
    //             012345678901234567890123456789
    char buff[] = "<device><test>   TEST</test></device>";
    xml_parser* parser = allocator_alloc(sizeof(xml_parser));
    parser->filename = NULL;
    parser->tree = btree_create(allocator_free);
    parser->file = test_malloc(sizeof(buff));
    memcpy(parser->file, buff, sizeof(buff));

    xml_parse(parser, buff);
    btree_node_t* tree_node = btree_get_root(parser->tree);
    btree_node_t* dev = btree_get_child(tree_node);
    btree_node_t* tes = btree_get_child(dev);

    xml_node* node2 = btree_get_data(tes);
    char* data = xml_string_to_char(node2->data);
    assert_string_equal(data, "TEST");
    test_free(data);

    xml_destroy(parser);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_read),
        cmocka_unit_test(get_begin_end_tag),
        cmocka_unit_test(get_open_close_tag),
        cmocka_unit_test(get_name_of_tag_test),
        cmocka_unit_test(get_end_tag_by_name_test),
        cmocka_unit_test(parse_test),
        cmocka_unit_test(file_parse_test),
        cmocka_unit_test(parse_data_test),
    };

    return cmocka_run_group_tests_name("queue_success_tests",tests, initialize, teardown);
}


static inline void assert_name(const char* fmt, btree_node_t* node)
{
    xml_node* n = btree_get_data(node);
    char* name = xml_string_to_char(n->name);
    assert_string_equal(name, fmt);
    test_free(name);
}

static inline void assert_data(const char* fmt, btree_node_t* node)
{
    xml_node* n = btree_get_data(node);
    char* name = xml_string_to_char(n->data);
    assert_string_equal(name, fmt);
    test_free(name);
}

static inline void assert_data_name(btree_node_t* node, const char* name, const char* data)
{
    assert_name(name, node);
    assert_data(data, node);
}
