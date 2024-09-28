#include "internals.h"

int CupI_isSubDelimiter(char ch) {
    switch(ch) {
        case '!':
        case '$':
        case '&':
        case '\'':
        case '(':
        case ')':
        case '*':
        case '+':
        case ',':
        case ';':
        case '=':
            return 1;

        default:
            return 0;
    }
}

void CupIIterator_init(char* src, struct CupIIterator* iter) {
    iter->index = 0;
    iter->src = src;
}

char CupIIterator_next(struct CupIIterator* iter) {
    char current = iter->src[iter->index];

    if(current != '\0') {
        iter->index += 1;
    }

    return current;
}

