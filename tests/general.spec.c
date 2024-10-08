#include "tests-new.h"

#include "Cup.h"

#include "tests.h"

typedef struct URL {
    char* scheme;
    char* username;
    char* password;
    char* host;
    int port;
    char* path;
    char* query;
    char* fragment;
} URL;

char generalBuffer[40];

#define TO_BE_URL(...)\
    CREATE_MATCHER(TO_BE_SPLIT_TO,\
        URL expected = { __VA_ARGS__ };\
        CupURL result;\
        MATCHER_CONDITION(PASSES_IF(Cup_parse(MATCHER_VALUE, &result) == 0)) {\
            MATCHER_FAIL(EXPECTED("Status 0"));\
        }\
        MATCHER_CONDITION(PASSES_IF(strncmp(MATCHER_VALUE + result.scheme.start, expected.scheme, result.scheme.length) == 0)) {\
            MATCHER_INFO("Got scheme '%s'\n", Cup_extractTo(MATCHER_VALUE, &result.scheme, generalBuffer, sizeof(generalBuffer)));\
            MATCHER_FAIL(EXPECTED(#__VA_ARGS__));\
        }\
        MATCHER_CONDITION(\
            PASSES_IF(\
                (!expected.username && result.username.length == 0)\
                || (expected.username && strncmp(MATCHER_VALUE + result.username.start, expected.username, result.username.length) == 0)\
            )\
        ) {\
            MATCHER_INFO("Got username '%s'\n", Cup_extractTo(MATCHER_VALUE, &result.username, generalBuffer, sizeof(generalBuffer)));\
            MATCHER_FAIL(EXPECTED(#__VA_ARGS__));\
        }\
        MATCHER_CONDITION(\
            PASSES_IF(\
                (!expected.password && result.password.length == 0)\
                || (expected.password && strncmp(MATCHER_VALUE + result.password.start, expected.password, result.password.length) == 0)\
            )\
        ) {\
            MATCHER_INFO("Got password '%s'\n", Cup_extractTo(MATCHER_VALUE, &result.password, generalBuffer, sizeof(generalBuffer)));\
            MATCHER_FAIL(EXPECTED(#__VA_ARGS__));\
        }\
        MATCHER_CONDITION(\
            PASSES_IF(\
                (!expected.host && result.host.length == 0)\
                || (expected.host && strncmp(MATCHER_VALUE + result.host.start, expected.host, result.host.length) == 0)\
            )\
        ) {\
            MATCHER_INFO("Got host '%s'\n", Cup_extractTo(MATCHER_VALUE, &result.host, generalBuffer, sizeof(generalBuffer)));\
            MATCHER_FAIL(EXPECTED(#__VA_ARGS__));\
        }\
        MATCHER_CONDITION(PASSES_IF(expected.port == result.port)) {\
            MATCHER_INFO("Got port %d\n", result.port);\
            MATCHER_FAIL(EXPECTED(#__VA_ARGS__));\
        }\
        MATCHER_CONDITION(\
            PASSES_IF(\
                (!expected.path && result.path.length == 0)\
                || (expected.path && strncmp(MATCHER_VALUE + result.path.start, expected.path, result.path.length) == 0)\
            )\
        ) {\
            MATCHER_INFO("Got path '%s'\n", Cup_extractTo(MATCHER_VALUE, &result.path, generalBuffer, sizeof(generalBuffer)));\
            MATCHER_FAIL(EXPECTED(#__VA_ARGS__));\
        }\
        MATCHER_CONDITION(\
            PASSES_IF(\
                (!expected.query && result.query.length == 0)\
                || (expected.query && strncmp(MATCHER_VALUE + result.query.start, expected.query, result.query.length) == 0)\
            )\
        ) {\
            MATCHER_INFO("Got query '%s'\n", Cup_extractTo(MATCHER_VALUE, &result.query, generalBuffer, sizeof(generalBuffer)));\
            MATCHER_FAIL(EXPECTED(#__VA_ARGS__));\
        }\
        MATCHER_CONDITION(\
            PASSES_IF(\
                (!expected.fragment && result.fragment.length == 0)\
                || (expected.fragment && strncmp(MATCHER_VALUE + result.fragment.start, expected.fragment, result.fragment.length) == 0)\
            )\
        ) {\
            MATCHER_INFO("Got fragment '%s'\n", Cup_extractTo(MATCHER_VALUE, &result.fragment, generalBuffer, sizeof(generalBuffer)));\
            MATCHER_FAIL(EXPECTED(#__VA_ARGS__));\
        }\
    )

DESCRIBE(general) {
    char generalBuffer[20];
    IT("works") {
        EXPECT("https://billy%20herington:aniki@memes.com/popular?topic=ricardo#latest") TO_BE_URL(
            "https",
            "billy%20herington",
            "aniki",
            "memes.com",
            -1,
            "/popular",
            "topic=ricardo",
            "latest"
        );

        EXPECT("urn:uuid:ksnflal") TO_BE_URL(
            "urn",
            NULL,
            NULL,
            NULL,
            -1,
            "uuid:ksnflal",
            NULL,
            NULL,
        );

        EXPECT("tel:+1-362-251-01-12") TO_BE_URL(
            "tel",
            NULL,
            NULL,
            NULL,
            -1,
            "+1-362-251-01-12",
            NULL,
            NULL,
        );


        EXPECT("file:///Cup.h") TO_BE_URL(
            "file",
            NULL,
            NULL,
            NULL,
            -1,
            "/Cup.h",
            NULL,
            NULL,
        );

        EXPECT("urn:uuid:11") TO_BE_URL(
            "urn",
            NULL,
            NULL,
            NULL,
            -1,
            "uuid:11",
            NULL,
            NULL
        )

        EXPECT("urn://uuid:11") TO_BE_URL(
            "urn",
            NULL,
            NULL,
            "uuid",
            11,
            NULL,
            NULL,
            NULL
        )
        
    }
}