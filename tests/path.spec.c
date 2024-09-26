#include "tests-new.h"

#include "internals.h"

#include "tests.h"

#undef TO_BE_PARSED_AS
#define TO_BE_PARSED_AS(EXPECTED_CODE, EXPECTED_VALUE)\
    CREATE_MATCHER(TO_BE_PARSED,\
        struct CupIIterator iter;\
        CupIIterator_init(MATCHER_VALUE, &iter);\
        CupRange result = { 0, 0 };\
        if(CupI_parse_path(&iter, &result) != EXPECTED_CODE) {\
            MATCHER_INFO("Invalid src");\
            MATCHER_FAIL(EXPECTED(#EXPECTED_VALUE))\
        }\
        MATCHER_CONDITION_S(\
            PASSES_IF(strncmp(MATCHER_VALUE + result.start, EXPECTED_VALUE, result.length) == 0),\
            EXPECTED(#EXPECTED_VALUE)\
        )\
    )

#undef TO_FAIL
#define TO_FAIL\
    CREATE_MATCHER(TO_FAIL,\
        struct CupIIterator iter;\
        CupIIterator_init(MATCHER_VALUE, &iter);\
        CupRange result = { 0, 0 };\
        MATCHER_CONDITION_S(\
            FAILS_IF(CupI_parse_path(&iter, &result) == 0),\
            NO_EXPECTED\
        )\
    )

DESCRIBE(path) {
    IT("works") {
        EXPECT("/memes/ss/dd") TO_BE_PARSED_AS(0, "/memes/ss/dd")
        EXPECT("/?") TO_BE_PARSED_AS(1, "/")
        EXPECT("/memes/ss/dd#") TO_BE_PARSED_AS(2, "/memes/ss/dd")
    }
}