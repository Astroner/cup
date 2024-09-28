#include <stdio.h>

#include "cup.h"

int main(void) {
    char href[] = "scheme://username:password@example.host:3000/example/path?with=query#and-fragment";
    
    CupURL url;

    Cup_parse(href, &url);

    printf("Scheme: %s\n", Cup_extract(href, &url.scheme));
    printf("Username: %s\n", Cup_extract(href, &url.username));
    printf("Password: %s\n", Cup_extract(href, &url.password));   
    printf("Host: %s\n", Cup_extract(href, &url.host));
    printf("Port: %d\n", url.port);
    printf("Path: %s\n", Cup_extract(href, &url.path));
    printf("Query: %s\n", Cup_extract(href, &url.query));
    printf("Fragment: %s\n", Cup_extract(href, &url.fragment));

    return 0;
}