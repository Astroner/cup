#include "cup.h"

#if !defined(CUP_INTERNALS_H)
#define CUP_INTERNALS_H

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
