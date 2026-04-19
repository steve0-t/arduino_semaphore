#include "../lib/XMLParser/XMLParser.h"
#include "../lib/Rstr/Rstr.h"
#include <unity.h>

static XMLParser parser = XMLParser();

void             test_invalid_normal_tag() {
    char buffer[256] = "<name>John</name>";
    rstr test        = {.data = buffer, .len = strlen(buffer)};

    auto res = parser.parse(test);
    TEST_ASSERT_EQUAL(BAD_FORMAT, res);
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_invalid_normal_tag);

    UNITY_END();
}
