# Hi There
This is **Cup** - C URL parsing library

# Table of content
 - [Quick example](#quick-example)
 - [Usage](#usage)
     - [Cup_parse](#cup_parse)
     - [Extracting values](#extracting-values)
         - [Cup_extract](#cup_extract)
         - [Cup_extractTo](#cup_extractto)
 - [Dependencies](#dependencies)

# Quick example
```c
#define CUP_IMPLEMENTATION
#include "Cup.h"

int main(void) {
    char href[] = "scheme://username:password@example.host:3000/example/path?with=query#and-fragment";
    
    CupURL url;

    Cup_parse(href, &url);

    printf("Scheme: %s\n", Cup_extract(href, &url.scheme));     // Scheme: scheme
    printf("Username: %s\n", Cup_extract(href, &url.username)); // Username: username
    printf("Password: %s\n", Cup_extract(href, &url.password)); // Password: password
    printf("Host: %s\n", Cup_extract(href, &url.host));         // Host: example.host
    printf("Port: %d\n", url.port);                             // Port: 3000
    printf("Path: %s\n", Cup_extract(href, &url.path));         // Path: /example/path
    printf("Query: %s\n", Cup_extract(href, &url.query));       // Query: with=query
    printf("Fragment: %s\n", Cup_extract(href, &url.fragment)); // Fragment: and-fragment

    return 0;
}
```
> Note that **Cup_extract()** allocates memory so you have to call **free** after.

# Usage
Cup is STB-like single header file library, so to use it you just need to include [Cup.h](https://raw.githubusercontent.com/Astroner/cup/master/Cup.h) into you project and define **CUP_IMPLEMENTATION** once before **include** statement to add the implementation.

## Cup_parse
Cup does not spit source string or allocate memory for new strings, but it builds a map of URL components represented mostly by **CupRange** structures. **CupRange** stores where the component starts and how long the component is.
```c
struct CupRange {
    int start;
    int length;
};
```
> If the components has length of 0 then it is not presented.

Complete URL information is stored in **CupURL** structure in the following format:
```c
struct CupURL {
    CupRange scheme;
    CupRange username;
    CupRange password;
    CupRange host;
    int port;
    CupRange path;
    CupRange query;
    CupRange fragment;
};
```

Function **Cup_parse()** takes source string and a pointer to the result **CupURL** structure
```c
int Cup_parse(char* src, CupURL* result);
```
**Cup_parse()** will return -1 if the src is not a correct url, 0 if everything is OK.

## Extracting values
Cup provides 2 functions to extract ranges from the source string: **Cup_extract()** and **Cup_extractTo()**

### Cup_extract
**Cup_extract()** takes source string and URL components **CupRange** and extracts information into newly allocated chunk of memory.
```c
char* Cup_extract(char* src, CupRange* range);
```
> **Cup_extract()** uses **malloc()** under the hood but this behavior can be changed by defining **CUP_STD_MALLOC** before implementation

### Cup_extractTo
**Cup_extractTo()** copies url component into provided buffer:
```c
char* Cup_extractTo(char* src, CupRange* range, char* buffer, int length);
```
 - **returns** - pointer to the provided **buffer**
 - **src** - source string
 - **range** - URL component
 - **buffer** - destination buffer
 - **length** - destination buffer size

**Cup_extractTo()** adds nul-terminator at the end, so the buffer size should be +1 from the component length.

# Dependencies
Cup uses several std functions like **malloc()** but this behavior can be changed by defining special macro before implementation to replace the default function.
The dependencies table is provided below:

| Macro          | Default value  |
|----------------|----------------|
| CUP_STD_MEMSET | memset         |
| CUP_STD_MEMCPY | memcpy         |
| CUP_STD_MALLOC | malloc         |
