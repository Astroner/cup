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
#if defined(CUP_IMPLEMENTATION)

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


int CupI_parse_scheme(struct CupIIterator* iter, CupRange* range) {    
    int length = 0;

    int badLastSymbol = 0;

    char ch;
    while((ch = CupIIterator_next(iter))) {
        if(CUP_IS_ALPHABETIC(ch)) {
            length++;
        } else if(CUP_IS_NUMERIC(ch)) {
            if(length == 0) { // digit cannot be the first char of a scheme
                return -1;
            }
            length++;
        } else if(ch == '-' || ch == '+' || ch == '.') {
            if(length == 0) { // + or - cannot be the first char of a scheme
                return -1;
            }
            badLastSymbol = -1; // + or - are allowed in the scheme, but cannot be last symbols
            length++;
            continue;
        } else if(ch == ':') {
            if(badLastSymbol) {
                return -1; // + or - are allowed in the scheme, but cannot be last symbols
            }

            range->length = length;

            return 0;
        } else {
            return -1; // unexpected character
        }

        badLastSymbol = 0;
    }

    return -1; // unexpected EOI
}

// returns 0 if ended with EOI
// 1 on /
// 2 on ?
// 3 on #
int CupI_parse_authority(struct CupIIterator* iter, CupURL* result) {
    // At first parser assumes that there is no user info and is looking for : or @ or any other not-host char to be proven wrong
    
    enum AuthPart {
        AuthPartHost,
        AuthPartPort,
        AuthPartUser,
        AuthPartPassword,
    } currentPart = AuthPartHost;

    int passedUserInfo = 0;

    int port = 0;

    int start = iter->index;
    int length = 0;

    int finishStatus = 0;

    char ch;
    while((ch = CupIIterator_next(iter))) {
        if(
            CUP_IS_ALPHABETIC(ch)
            || ch == '-'
            || ch == '.'
        ) {
                length++;
            if(currentPart == AuthPartPort) {
                // if we have not passed user info yet, then we assume that it is password and not port. and the previous part was password
                if(passedUserInfo) {
                    return -1; // Unexpected symbol inside of the port
                }
                passedUserInfo = 1;
                currentPart = AuthPartPassword;
                result->username = result->host;
                result->host.start = 0;
                result->host.length = 0;
                port = 0;
            }
        } else if(CUP_IS_NUMERIC(ch)) {
            length++;
            if(currentPart == AuthPartPort) {
                port = port * 10 + ch - '0';
            }
        } else if(ch == '@') {
            switch(currentPart) {
                // Same with colon. If we see @ during the host parsing, we assume that it was username all the time and continue
                case AuthPartHost:
                case AuthPartUser:
                    result->username.start = start;
                    result->username.length = length;
                    start = iter->index;
                    length = 0;
                    currentPart = AuthPartHost;
                    passedUserInfo = 1;
                    break;

                case AuthPartPassword:
                    result->password.start = start;
                    result->password.length = length;
                    start = iter->index;
                    length = 0;
                    currentPart = AuthPartHost;
                    break;

                // Same as if we encounter smth unexpected during port parsing. We assume that we've been parsing user info all this time
                case AuthPartPort:
                    result->username = result->host;
                    result->host.start = 0;
                    result->host.length = 0;

                    result->password.start = start;
                    result->password.length = length;
                    start = iter->index;
                    length = 0;
                    currentPart = AuthPartHost;
                    port = 0;

                    passedUserInfo = 1;
                    break;
            }
        } else if(ch == ':') {
            if(currentPart == AuthPartHost) {
                if(length == 0) {
                    return -1; // Port cannot be specified without host. Example: http://:33
                }
                result->host.start = start;
                result->host.length = length;
                currentPart = AuthPartPort;
                start = iter->index;
                length = 0;
            } else {
                // ':' is expected(outside of IPv6) only after username and host, 
                // but current parser never starts from username, so outside of the host case it is unexpected
                return -1; 
            }
        } else if(CupI_isSubDelimiter(ch)) {
            length++;
            if(currentPart == AuthPartHost) {
                currentPart = AuthPartUser;
                passedUserInfo = 1;
            } else if(currentPart == AuthPartPort) {
                result->username = result->host;
                result->host.start = 0;
                result->host.length = 0;

                currentPart = AuthPartPassword;
                passedUserInfo = 1;
                port = 0;
            }
        } else if(ch == '/') {
            finishStatus = 1;
            break;
        } else if(ch == '?') {
            finishStatus = 2;
            break;
        } else if(ch == '#') {
            finishStatus = 3;
            break;
        } else if(ch == '%') {
            ch = CupIIterator_next(iter);
            if(!CUP_IS_HEX(ch)) return -1; // wrong percent encoded

            ch = CupIIterator_next(iter);
            if(!CUP_IS_HEX(ch)) return -1; // wrong percent encoded
        } else {
            return -1; // Unexpected symbol
        }
    }

    if(currentPart == AuthPartHost) {
        result->host.start = start;
        result->host.length = length;

        return finishStatus;
    }

    if(currentPart == AuthPartPort) {
        result->port = port;

        return finishStatus;
    }

    return -1; // unexpected EOI
}

// returns 0 if ends with EOI
// returns 1 if ends with ?
// returns 2 if ends with #
int CupI_parse_path(struct CupIIterator* iter, CupRange* result) {
    result->start = iter->index;
    int length = 0;

    char ch;
    while((ch = CupIIterator_next(iter))) {
        if(ch == '?') {
            result->length = length;
            return 1;
        } else if(ch == '#') {
            result->length = length;
            return 2;
        } else if(
            CUP_IS_UNRESERVED(ch)
            || ch == '/'
            || ch == '@'
            || ch == ':'
            || CupI_isSubDelimiter(ch)
        ) {
            length++;
        } else if(ch == '%') {
            ch = CupIIterator_next(iter);
            if(!CUP_IS_HEX(ch)) return -1; // wrong percent encoded

            ch = CupIIterator_next(iter);
            if(!CUP_IS_HEX(ch)) return -1; // wrong percent encoded
        } else {
            return -1; // unexpected symbol
        }
    }

    result->length = length;

    return 0;
}

// returns 0 if ends with EOI
// returns 1 if ends with #
int CupI_parse_query(struct CupIIterator* iter, CupRange* result) {
    result->start = iter->index;
    int length = 0;

    char ch;
    while((ch = CupIIterator_next(iter))) {
        if(ch == '#') {
            result->length = length;
            return 1;
        } else if(
            CUP_IS_UNRESERVED(ch)
            || ch == '/'
            || ch == '@'
            || ch == ':'
            || ch == '?'
            || CupI_isSubDelimiter(ch)
        ) {
            length++;
        } else if(ch == '%') {
            ch = CupIIterator_next(iter);
            if(!CUP_IS_HEX(ch)) return -1; // wrong percent encoded

            ch = CupIIterator_next(iter);
            if(!CUP_IS_HEX(ch)) return -1; // wrong percent encoded
        } else {
            return -1; // unexpected symbol
        }
    }

    result->length = length;

    return 0;
}

int CupI_parse_fragment(struct CupIIterator* iter, CupRange* result) {
    result->start = iter->index;
    int length = 0;

    char ch;
    while((ch = CupIIterator_next(iter))) {
        if(
            CUP_IS_UNRESERVED(ch)
            || ch == '/'
            || ch == '@'
            || ch == ':'
            || ch == '?'
            || CupI_isSubDelimiter(ch)
        ) {
            length++;
        } else if(ch == '%') {
            ch = CupIIterator_next(iter);
            if(!CUP_IS_HEX(ch)) return -1; // wrong percent encoded

            ch = CupIIterator_next(iter);
            if(!CUP_IS_HEX(ch)) return -1; // wrong percent encoded
        } else {
            return -1; // unexpected char
        }
    }

    result->length = length;

    return 0;
}

int Cup_parse(char* src, CupURL* result) {
    CUP_STD_MEMSET(result, 0, sizeof(*result));

    struct CupIIterator iter;
    CupIIterator_init(src, &iter);

    if(CupI_parse_scheme(&iter, &result->scheme) < 0) {
        return -1;
    }

    int status;
    int backPath = 1;
    if(CupIIterator_next(&iter) == '/') {
        backPath = 2;
        if(CupIIterator_next(&iter) == '/') {
            if((status = CupI_parse_authority(&iter, result)) < 0) {
                return -1;
            }

            if(status == 0) {
                return 0;
            }

            if(status == 2) {
                goto parse_query;
            }

            if(status == 3) {
                goto parse_fragment;
            }

            // status 1 means auth ended with / so we go back 1 char to include it to path
            backPath = 1;
        }
    }

    status = CupI_parse_path(&iter, &result->path);

    if(status < 0) {
        return -1;
    }

    result->path.start -= backPath;
    result->path.length += backPath;
    
    if(status == 0) {
        return 0;
    }


    if(status == 1) {
parse_query:
        if((status = CupI_parse_query(&iter, &result->query)) < 0) {
            return -1;
        }

        if(status == 0) {
            return 0;
        }
    }

parse_fragment:
    CupI_parse_fragment(&iter, &result->fragment);

    return 0;
}

char* Cup_extract(char* src, CupRange* range) {
    if(range->length == 0) {
        return NULL;
    }

    char* str = CUP_STD_MALLOC(range->length + 1);

    if(!str) return NULL;

    CUP_STD_MEMCPY(str, src + range->start, range->length);
    str[range->length] = '\0';

    return str;
}

char* Cup_extractTo(char* src, CupRange* range, char* buffer, int length) {
    if(length < range->length + 1) {
        buffer[0] = '\0';
        return NULL;
    }

    CUP_STD_MEMCPY(buffer, src + range->start, range->length);
    buffer[range->length] = '\0';

    return buffer;
}
#endif // CUP_IMPLEMENTATION
