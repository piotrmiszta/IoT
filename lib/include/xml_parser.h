#ifndef LIB_XML_PARSER_H_
#define LIB_XML_PARSER_H_

typedef struct xml_parser xml_parser;

xml_parser* xml_create(const char* filename);
void xml_destroy(xml_parser* parser);

#endif
