#include <stdio.h>

#include "cup.h"

int main(void) {

    char url[] = "https://username:password@google.com:4040";

    CupURL result;

    if(Cup_parse(url, &result) < 0) {
        printf("failed\n");
    }

    printf(
        "scheme: '%s'\n"
        "username: '%s'\n"
        "password: '%s'\n"
        "host: '%s'\n"
        "port: '%d'\n"
        "path: '%s'\n"
        "query: '%s'\n"
        "fragment: '%s'\n",
        Cup_extract(url, &result.scheme),
        Cup_extract(url, &result.username),
        Cup_extract(url, &result.password),
        Cup_extract(url, &result.host),
        result.port,
        Cup_extract(url, &result.path),
        Cup_extract(url, &result.query),
        Cup_extract(url, &result.fragment)
    );

    return 0;
}
