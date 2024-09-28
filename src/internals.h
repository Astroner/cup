#include "cup.h"

#if !defined(CUP_INTERNALS_H)
#define CUP_INTERNALS_H

#if !defined(CUP_STD_MEMSET)
    #include <string.h>
    #define CUP_STD_MEMSET memset
#endif // CUP_STD_MEMSET

#if !defined(CUP_STD_MEMCPY)
    #include <string.h>
    #define CUP_STD_MEMCPY memcpy
#endif // CUP_STD_MEMCPY

#if !defined(CUP_STD_MALLOC)
    #include <stdlib.h>
    #define CUP_STD_MALLOC malloc
#endif // CUP_STD_MALLOC


#define CUP_IS_ALPHABETIC(ch) (((ch) >= 'a' && (ch) <= 'z') || ((ch) >= 'A' && (ch) <= 'Z'))
#define CUP_IS_NUMERIC(ch) ((ch) >= '0' && (ch) <= '9')
#define CUP_IS_SPECIAL(ch) ((ch) == '-' || (ch) == '.' || (ch) == '_' || (ch) == '~')
#define CUP_IS_UNRESERVED(ch) (CUP_IS_ALPHABETIC(ch) || CUP_IS_NUMERIC(ch) || CUP_IS_SPECIAL(ch))

#define CUP_IS_HEX(ch) (CUP_IS_NUMERIC(ch) || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'))

int CupI_isSubDelimiter(char ch);

struct CupIIterator {
    char* src;
    int index;
};

void CupIIterator_init(char* src, struct CupIIterator* iter);
char CupIIterator_next(struct CupIIterator* iter);
int CupI_parse_scheme(struct CupIIterator* iter, CupRange* range);
int CupI_parse_path(struct CupIIterator* iter, CupRange* result);
int CupI_parse_authority(struct CupIIterator* iter, CupURL* result);

#endif // CUP_INTERNALS_H
