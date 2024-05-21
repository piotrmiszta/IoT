#include <string.h>
#include "xml_parser.h"
#include "allocator.h"
#include "btree.h"

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
    char* start;
    char* end;
    xml_string name;
    xml_string data;
}xml_node;

static inline char* xml_string_to_char(const xml_string string);

static inline const char* skip_whitespace(const char* buffer);
static inline const char* get_open_tag(const char* buffer);
static inline const char* get_close_tag(const char* buffer);
static inline const char* get_begin_of_tag(const char* buffer);
static inline const char* get_end_of_tag(const char* buffer);
static inline const char* get_close_tag_by_name(const char* buff, const xml_string string);
static inline bool xml_string_cmp(const xml_string a, const xml_string b);

static xml_parser* xml_alloc(const char* filename);
static i32 xml_parse(xml_parser* parser, const char* buff);
static char* xml_read(const char* filename);
static xml_node* xml_alloc_node(void);

void xml_destroy(xml_parser* parser)
{
    allocator_free(parser->file);
    btree_destroy(parser->tree);
    allocator_free(parser->file);
}

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
    parser->file = file;

    xml_parse(parser, NULL);
    return parser;
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
    parser->tree = btree_create(NULL); //change this function to deall
    if(!parser->tree)
    {
        allocator_free(parser->filename);
        allocator_free(parser);
        return NULL;
    }
    return parser;
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

/*

    <device>
        <interfaces>
            <test>Testing data</test>
            <test2></test2>
        </interfaces>
        <protocols>
        </protocols>
    </device>


    We just want to parse like this,
    find first open tag "device" and closed tag "/device",
    and add this to tree as
*/
//TODO: implement this function

static i32 xml_parse(xml_parser* parser, const char* buff)
{
    (void)parser;
    (void)buff;
    void* ptr = xml_alloc_node();
    allocator_free(ptr);
    return 0;
    /*
    const char* begin = buff;
    const char* open_tag = buff;
    const char* end_tag = buff;
    btree_node_t* root = btree_get_root(parser->tree);
    int k = 0;  //0 for left, 1 for right
    while(1)
    {
        // get open tag
        open_tag = get_open_tag(open_tag);
        xml_string name = get_tag_name(open_tag);
        end_tag = get_close_tag_by_name(buff, name);
        xml_node* node = xml_alloc_node();
        node->data = (xml_string){NULL, NULL};
        node->start = open_tag;
        node->end = end_tag;
        node->name = (xml_string){name.begin, name.end};
        if(k == 0)  //this is new child
        {
            btree_add_left(root, node);
            root = btree_get_left(root);
        }
        else        //this is new subling
        {
            btree_add_right(root, node);
            root = btree_get_right(root);
        }
    }
    return 0;*/
}

static inline char* xml_string_to_char(const xml_string string)
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

static inline bool xml_string_cmp(const xml_string a, const xml_string b)
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