#include "tests-new.h"

#include "internals.h"

#include "tests.h"

char authBuffer[40];

#define TO_BE_SPLIT_TO(STATUS, USR, PASS, HOST, PORT)\
    CREATE_MATCHER(TO_BE_SPLIT_TO,\
        struct CupIIterator iter;\
        CupIIterator_init(MATCHER_VALUE, &iter);\
        CupURL result;\
        memset(&result, 0, sizeof(result));\
        int status = CupI_parse_authority(&iter, &result);\
        if(status != STATUS) {\
            MATCHER_FAIL(EXPECTED("Status " #STATUS));\
        }\
        if(!*USR && result.username.length != 0) {\
            MATCHER_FAIL(EXPECTED("username to be NULL"));\
        }\
        MATCHER_CONDITION(PASSES_IF(strncmp(MATCHER_VALUE + result.username.start, USR, result.username.length) == 0)) {\
            MATCHER_INFO("Got username '%s'\n", Cup_extractTo(MATCHER_VALUE, &result.username, authBuffer, sizeof(authBuffer)));\
            MATCHER_FAIL(EXPECTED("username to be " #USR));\
        }\
        if(!*PASS && result.password.length != 0) {\
            MATCHER_FAIL(EXPECTED("password to be NULL"));\
        }\
        MATCHER_CONDITION(PASSES_IF(strncmp(MATCHER_VALUE + result.password.start, PASS, result.password.length) == 0)) {\
            MATCHER_INFO("Got password '%s'\n", Cup_extractTo(MATCHER_VALUE, &result.password, authBuffer, sizeof(authBuffer)));\
            MATCHER_FAIL(EXPECTED("password to be " #PASS));\
        }\
        if(!*HOST && result.host.length != 0) {\
            MATCHER_FAIL(EXPECTED("host to be NULL"));\
        }\
        MATCHER_CONDITION(PASSES_IF(strncmp(MATCHER_VALUE + result.host.start, HOST, result.host.length) == 0)) {\
            MATCHER_INFO("Got host '%s'\n", Cup_extractTo(MATCHER_VALUE, &result.host, authBuffer, sizeof(authBuffer)));\
            MATCHER_FAIL(EXPECTED("host to be " #HOST));\
        }\
        MATCHER_CONDITION(PASSES_IF(PORT == result.port)) {\
            MATCHER_INFO("Got port %d\n", result.port);\
            MATCHER_FAIL(EXPECTED("port to be " #PORT));\
        }\
    )

DESCRIBE(authority) {
    IT("works") {
        EXPECT("username:password@google.com:4040") TO_BE_SPLIT_TO(
            0,
            "username",
            "password",
            "google.com",
            4040
        )
        
        EXPECT("username@google.com:4040") TO_BE_SPLIT_TO(
            0,
            "username",
            "",
            "google.com",
            4040
        )

        EXPECT("0.0.0.0:4040#") TO_BE_SPLIT_TO(
            3,
            "",
            "",
            "0.0.0.0",
            4040
        )

        EXPECT("example.com:4040@example.com:4040/") TO_BE_SPLIT_TO(
            1,
            "example.com",
            "4040",
            "example.com",
            4040
        )
    }
}