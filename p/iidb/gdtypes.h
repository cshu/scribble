
struct DLLInt {
    int value;
    struct DLLInt *prev;
    struct DLLInt *next;
};

struct LLInt {
    int value;
    struct LLInt *next;
};

struct CArr {
    char *ch;
    size_t size;
};

struct UCArr {
    unsigned char *ch;
    size_t size;
};

enum CheckUTF16StatResult {
    FIRST_BYTE_ALL_ZERO,
    SECOND_BYTE_ALL_ZERO,
    OTHER_RESULT,
    INVALID_UTF16
};
