#include "tests-new.h"

#include <string.h>

#include "tests.h"

#include "cup.h"
#include "internals.h"

#define TO_BE_PARSED_AS(EXPECTED_VALUE)\
    CREATE_MATCHER(TO_BE_PARSED,\
        struct CupIIterator iter;\
        CupIIterator_init(MATCHER_VALUE, &iter);\
        CupRange result = { 0, 0 };\
        if(CupI_parse_scheme(&iter, &result) != 0) {\
            MATCHER_INFO("Invalid src");\
            MATCHER_FAIL(EXPECTED(#EXPECTED_VALUE))\
        }\
        MATCHER_CONDITION_S(\
            PASSES_IF(strncmp(MATCHER_VALUE + result.start, EXPECTED_VALUE, result.length) == 0),\
            EXPECTED(#EXPECTED_VALUE)\
        )\
    )

#define TO_FAIL\
    CREATE_MATCHER(TO_FAIL,\
        struct CupIIterator iter;\
        CupIIterator_init(MATCHER_VALUE, &iter);\
        CupRange result = { 0, 0 };\
        MATCHER_CONDITION_S(\
            FAILS_IF(CupI_parse_scheme(&iter, &result) == 0),\
            NO_EXPECTED\
        )\
    )

DESCRIBE(scheme) {
    IT("works") {
        EXPECT("aa://google.com") TO_BE_PARSED_AS("aa");
        EXPECT("a+b://google.com") TO_BE_PARSED_AS("a+b");
        EXPECT("a-b://google.com") TO_BE_PARSED_AS("a-b");
        EXPECT("a-00://google.com") TO_BE_PARSED_AS("a-00");
        EXPECT("http://google.com") TO_BE_PARSED_AS("http");
        EXPECT("https://google.com") TO_BE_PARSED_AS("https");
    }

    IT("fails") {
        EXPECT("+ss://google.com") TO_FAIL;
        EXPECT("ss+://google.com") TO_FAIL;
        EXPECT("00a://google.com") TO_FAIL;
    }
}