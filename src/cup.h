#if !defined(URL_PARSE)
#define URL_PARSE

typedef struct CupRange {
    int start;
    int length;
} CupRange;

typedef struct CupURL {
    CupRange scheme;
    CupRange username;
    CupRange password;
    CupRange host;
    int port;
    CupRange path;
    CupRange query;
    CupRange fragment;
} CupURL;

int Cup_parse(char* src, CupURL* result);
char* Cup_extract(char* src, CupRange* range);
char* Cup_extractTo(char* src, CupRange* range, char* buffer, int length);

#endif // URL_PARSE
