/**
 * @brief   Very simple xml parser, goal is only to have description about devices used in network,
 *          Parser read all xml file, next are parsing to binary tree all file, all data are stored in readed file,
 *          so only one time memory is allocated. xml_string structure that contains string, have two pointers,
 *          begin of string in file and end of string in file.
 *          If we need to store string and deallocate all memory, we need to use xml_string_to_char() function,
 *          that allocate memory for string, copy string from file buffer. (Remember to free data allocated by this function)
 *
 *          This parser not need to be very fast and efficient, since they will be used only in boot, or close
 *          stage of program executon.
 *
 *          TODO: just add parsing in diffrent way, from btree to xml file
*/

#ifndef LIB_XML_PARSER_H_
#define LIB_XML_PARSER_H_
#include "btree.h"

/**
 * @brief contains all information needed by parser
*/
typedef struct xml_parser xml_parser;

/**
 * @brief contains node information about xml element - tag
*/
typedef struct xml_node xml_node;

/**
 * @brief string used in xml parsing
*/
typedef struct xml_string xml_string;

/**
 * @brief alloc xml_parser, find and parse xml file
 * @return 0 on success, or error code
*/
xml_parser* xml_create(const char* filename);

/**
 * @brief dealloc xml_parser and all related memory
*/
void xml_destroy(xml_parser* parser);

/**
 * @brief return root - first element of parsed tree
*/
btree_node_t* xml_get_root(const xml_parser* parser);

/**
 * @brief return data of node
*/
xml_string xml_node_data(const xml_node* node);

/**
 * @brief return name of node
*/
xml_string xml_node_name(const xml_node* node);

/**
 * @brief return pointer to new allocated memory and setted by string
*/
char* xml_string_to_char(const xml_string string);

/**
 * @brief compare two xml_strings
*/
bool xml_string_cmp(const xml_string a, const xml_string b);

#endif
