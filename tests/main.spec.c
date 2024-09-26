#include "tests-new.h"

#define CUP_IMPLEMENTATION
#include "../Cup.h"

#define MULTI_TEST
#define WITH_DESCRIBE_MERGE
#include "tests.h"

#include "scheme.spec.c"
#include "path.spec.c"
#include "authority.spec.c"
#include "general.spec.c"

DESCRIBE_MERGE(internal_parsers, scheme, path, authority)

RUN_TESTS(internal_parsers, general)
