#include <string.h>
#include "xml_parser.h"
#include "allocator.h"
#include "btree.h"
#include <string.h>

typedef struct xml_string
{
    const char* begin;
    const char* end;
}xml_string;

typedef struct xml_parser
{
    char* filename;
    btree_t* tree;
    char* file;
}xml_parser;

typedef struct xml_node
{
    const char* start;
    const char* end;
    xml_string name;
    xml_string data;
}xml_node;

static inline const char* skip_whitespace(const char* buffer);
static inline const char* get_open_tag(const char* buffer);
static inline const char* get_close_tag(const char* buffer);
static inline const char* get_begin_of_tag(const char* buffer);
static inline const char* get_end_of_tag(const char* buffer);
static inline const char* get_close_tag_by_name(const char* buff, const xml_string string);
static inline xml_string get_tag_name(const char* buffer);
static xml_string get_tag_data(const char* begin, const char* end);

static xml_parser* xml_alloc(const char* filename);
static i32 xml_parse(xml_parser* parser, const char* buff);
static char* xml_read(const char* filename);
static xml_node* xml_alloc_node(void);
static inline btree_node_t* add_object(btree_node_t* parent, xml_node* n, bool* is_child);
static i32 xml_get_all_childs(btree_node_t* parent, const char* buff, const char* end_buff);

/* global functions */
xml_parser* xml_create(const char* filename)
{
    xml_parser* parser = xml_alloc(filename);
    if(!parser)
    {
        print_err("Cannot allocate memory for xml_parser\n");
        return NULL;
    }
    /* start  processing file*/
    char* file = xml_read(filename);
    if(!file)
    {
        print_err("Cannot read xml file errno: %s", strerror(errno));
    }
    parser->file = file;
    xml_parse(parser, file);
    return parser;
}

void xml_destroy(xml_parser* parser)
{
    allocator_free(parser->file);
    btree_destroy(parser->tree);
    allocator_free(parser->filename);
    allocator_free(parser);
}

btree_node_t* xml_get_root(const xml_parser* parser)
{
    return btree_get_root(parser->tree);
}

xml_string xml_node_data(const xml_node* node)
{
    return node->data;
}

xml_string xml_node_name(const xml_node* node)
{
    return node->name;
}

char* xml_string_to_char(const xml_string string)
{
    size_t len = (string.end - string.begin) + 1;
    char* buff = allocator_calloc(len, sizeof(char));
    if(!buff)
    {
        return NULL;
    }
    memcpy(buff, string.begin, len - 1);
    return buff;
}

bool xml_string_cmp(const xml_string a, const xml_string b)
{
    u32 lena = a.end - a.begin;
    u32 lenb = b.end - b.begin;
    if(lena != lenb)
    {
        return false;
    }
    const char* ptr1 = a.begin;
    const char* ptr2 = b.begin;

    for(u32 i = 0; i < lena; i++)
    {
        if(ptr1[i] != ptr2[i])
        {
            return false;
        }
    }

    return true;
}

/* static functions */
static inline const char* skip_whitespace(const char* buffer)
{
    do
    {
        switch (*buffer)
        {
        case '\0':
            return NULL;
        case '\n':
        case '\t':
        case '\r':
        case ' ':
            buffer++;
            break;
        default:
            return buffer;
        }
    }while(1);
}

static inline const char* get_open_tag(const char* buffer)
{
    do
    {
        buffer = get_begin_of_tag(buffer);
        if(buffer)
        {
            if(*(buffer + 1) != '/')
            {
                return buffer;
            }
            else
            {
                buffer++;
                buffer++;
            }
        }
        else
        {
            return NULL;
        }
    }while(1);

}

static inline const char* get_close_tag(const char* buffer)
{
    do
    {
        buffer = get_begin_of_tag(buffer);
        if(buffer)
        {
            if(*(buffer + 1) == '/')
            {
                return buffer;
            }
            else
            {
                buffer = get_end_of_tag(buffer);
            }
        }
        else
        {
            return NULL;
        }
    }while(1);

}

static inline const char* get_begin_of_tag(const char* buffer)
{
    do
    {
        switch (*buffer)
        {
        case '\0':
            return NULL;
        case '<':
            return buffer;
        default:
            buffer++;
        }
    }while(1);
}

static inline const char* get_end_of_tag(const char* buffer)
{
    do
    {
        switch (*buffer)
        {
        case '\0':
            return NULL;
        case '>':
            return buffer;
        default:
            buffer++;
        }
    }while(1);
}

static inline const char* get_close_tag_by_name(const char* buff, const xml_string string)
{
    do
    {
        buff = get_close_tag(buff);
        xml_string t = get_tag_name(buff);
        if(xml_string_cmp(string, t))
        {
            return buff;
        }
        else
        {
            buff = get_close_tag(get_end_of_tag(buff));
        }
    } while (1);

}

static inline xml_string get_tag_name(const char* buffer)
{
    const char* begin = get_begin_of_tag(buffer);
    const char* end = get_end_of_tag(begin);
    if(*(begin + 1) == '/')
    {
        begin++;
    }
    return (xml_string){begin + 1, end};
}

static xml_string get_tag_data(const char* begin, const char* end)
{
    const char* bend =get_end_of_tag(begin) + 1;
    const char* next = get_open_tag(bend);
    bend = skip_whitespace(bend);
    if(next > end || next == NULL)
    {
        return (xml_string){bend, end};
    }
    else
    {
        return (xml_string){NULL, NULL};
    }
}

static xml_parser* xml_alloc(const char* filename)
{
    assert(filename);
    xml_parser* parser = allocator_alloc(sizeof(xml_parser));
    if(!parser)
    {
        return NULL;
    }
    u32 filenamesz = strlen(filename) + 1;
    parser->filename = allocator_alloc(filenamesz);
    if(!parser->filename)
    {
        allocator_free(parser);
        return NULL;
    }
    memcpy(parser->filename, filename, filenamesz);
    parser->tree = btree_create(allocator_free); //change this function to deall
    if(!parser->tree)
    {
        allocator_free(parser->filename);
        allocator_free(parser);
        return NULL;
    }
    return parser;
}

static i32 xml_parse(xml_parser* parser, const char* buff)
{
    return xml_get_all_childs(btree_get_root(parser->tree), buff, &buff[strlen(buff)]);;
}

static char* xml_read(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if(!file)
    {
        print_err("Can't open file %s\n", filename);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    i32 len = ftell(file);
    len++;
    char* temp = allocator_alloc(len * sizeof(char));
    if(!temp)
    {
        print_err("Cannot malloc buffer for file\n");
        return NULL;
    }
    fseek(file, SEEK_SET, 0);
    i32 t = fread(temp, sizeof(char), len, file);
    (void)t;
    fclose(file);
    temp[len - 1] = '\0';
    return temp;
}

static xml_node* xml_alloc_node(void)
{
    xml_node* node = allocator_alloc(sizeof(xml_node));
    if(!node)
    {
        print_err("cannot alloc memory for node\n");
        return NULL;
    }
    return node;
}

static inline btree_node_t* add_object(btree_node_t* parent, xml_node* n, bool* is_child)
{
    btree_node_t* ret = NULL;
    if(parent)
    {
        if(*is_child)
        {
            ret = btree_add_sibling(parent, n);
        }
        else
        {
            ret = btree_add_child(parent, n);
            *is_child = true;

        }
    }
    return ret;
}

static i32 xml_get_all_childs(btree_node_t* parent, const char* buff, const char* end_buff)
{
    bool is_child = false;
    btree_node_t* active = parent;
    const char* open = buff;
    const char* close = buff;
    while(1)
    {
        // get child, call this function for child,
        // get another child -> sibling for previous child and repeat till end
        open = get_open_tag(open);
        if(!open || open >= end_buff)
        {
            return 0;
        }
        xml_string name = get_tag_name(open);
        close = get_close_tag_by_name(open, name);
        xml_node* node = xml_alloc_node();
        node->data = get_tag_data(open, close);
        node->name = (xml_string){name.begin,  name.end};
        node->start = open;
        node->end = close;
        active = add_object(active, node, &is_child);
        xml_get_all_childs(active, get_end_of_tag(open), close);
        open = close;
    }
}
