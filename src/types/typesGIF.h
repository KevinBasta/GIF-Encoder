
typedef struct RGB {
    u8 red;
    u8 green;
    u8 blue;
} RGB;

typedef struct colorTable {
    u32 size;
    RGB *arr;
} colorTable;

typedef struct codeTable {
    u32 index;
    HashMap *map;
} codeTable;