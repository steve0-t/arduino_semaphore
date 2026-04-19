#include "../lib/XMLParser/XMLParser.h"
#include "../lib/Rstr/Rstr.h"
#include <unity.h>

void test_invalid_normal_tag() {
    XMLParser parser      = XMLParser();
    char      buffer[256] = "<name>John</name>";
    rstr      test        = {.data = buffer, .len = (uint16_t)strlen(buffer)};

    auto      res = parser.parse(test);
    TEST_ASSERT_EQUAL(BAD_FORMAT, res);
}

void test_invalid_complicated_tag() {
    XMLParser parser = XMLParser();
    char      buffer[256] =
        "<dependency> <groupId>org.springframework.boot</groupId>"
        "<artifactId>spring-boot-starter</artifactId> </dependency>";
    rstr test = {.data = buffer, .len = (uint16_t)strlen(buffer)};

    auto res = parser.parse(test);
    TEST_ASSERT_EQUAL(BAD_FORMAT, res);
}

void test_buffer_overflow() {
    XMLParser parser = XMLParser();
    char      buffer[512] =
        "dependencies> <dependency> "
        "<groupId>org.springframework.boot</groupId> "
        "<artifactId>spring-boot-starter</artifactId> </dependency> "
        "<dependency> <groupId>org.springframework.boot</groupId> "
        "<artifactId>spring-boot-starter-test</artifactId> <scope>test</scope> "
        "</dependency> <dependency> "
        "<groupId>org.springframework.boot</groupId> "
        "<artifactId>spring-boot-devtools</artifactId> <scope>runtime</scope> "
        "</dependency>";
    rstr test = {.data = buffer, .len = (uint16_t)strlen(buffer)};

    auto res = parser.parse(test);
    TEST_ASSERT_EQUAL(BAD_FORMAT, res);
}

void test_empty_string() {
    XMLParser parser    = XMLParser();
    char      buffer[1] = "";
    rstr      test      = {.data = buffer, .len = (uint16_t)strlen(buffer)};

    auto      res = parser.parse(test);
    TEST_ASSERT_EQUAL(BAD_FORMAT, res);
}

void test_comment() {
    XMLParser parser     = XMLParser();
    char      buffer[32] = "<!--this is a comment-->";
    rstr      test       = {.data = buffer, .len = (uint16_t)strlen(buffer)};

    auto      res = parser.parse(test);
    TEST_ASSERT_EQUAL(COMMENT, res);
}

void test_bad_states() {
    XMLParser parser      = XMLParser();
    char      buffer1[64] = "<cmd type=\"set\" state=\"SUPER\"/>";
    char      buffer2[64] = "<cmd type=\"set\" state=\"ARDUINO\"/>";
    char      buffer3[64] = "<cmd type=\"set\" state=\"COMPILER\"/>";
    char      buffer4[64] = "<cmd type=\"set\" state=\"PARSER\"/>";
    char      buffer5[64] = "<cmd type=\"set\" state=\"ARM\"/>";

    rstr      test1 = {.data = buffer1, .len = (uint16_t)strlen(buffer1)};
    rstr      test2 = {.data = buffer2, .len = (uint16_t)strlen(buffer2)};
    rstr      test3 = {.data = buffer3, .len = (uint16_t)strlen(buffer3)};
    rstr      test4 = {.data = buffer4, .len = (uint16_t)strlen(buffer4)};
    rstr      test5 = {.data = buffer5, .len = (uint16_t)strlen(buffer5)};

    auto      res1 = parser.parse(test1);
    auto      res2 = parser.parse(test2);
    auto      res3 = parser.parse(test3);
    auto      res4 = parser.parse(test4);
    auto      res5 = parser.parse(test5);

    TEST_ASSERT_EQUAL(BAD_STATE, res1);
    TEST_ASSERT_EQUAL(BAD_STATE, res2);
    TEST_ASSERT_EQUAL(BAD_STATE, res3);
    TEST_ASSERT_EQUAL(BAD_STATE, res4);
    TEST_ASSERT_EQUAL(BAD_STATE, res5);
}

void test_ping_pong() {
    XMLParser parser     = XMLParser();
    char      buffer[64] = "<cmd type=\"ping\"/>";

    rstr      test = {.data = buffer, .len = (uint16_t)strlen(buffer)};

    auto      res = parser.parse(test);

    TEST_ASSERT_EQUAL(parser.getCmpType(), PING);
    TEST_ASSERT_EQUAL(parser.getState(), SCOUNT);
    TEST_ASSERT_EQUAL_STRING(parser.getFields().type, "ping");
}

void test_valid_states() {
    XMLParser parser1 = XMLParser();
    XMLParser parser2 = XMLParser();
    XMLParser parser3 = XMLParser();
    XMLParser parser4 = XMLParser();
    XMLParser parser5 = XMLParser();

    char      buffer1[64] = "<cmd type=\"set\" state=\"STOP\"/>";
    char      buffer2[64] = "<cmd type=\"set\" state=\"READY\"/>";
    char      buffer3[64] = "<cmd type=\"set\" state=\"GO\"/>";
    char      buffer4[64] = "<cmd type=\"set\" state=\"CAUTION\"/>";
    char      buffer5[64] = "<cmd type=\"set\" state=\"OFF\"/>";

    rstr      test1 = {.data = buffer1, .len = (uint16_t)strlen(buffer1)};
    rstr      test2 = {.data = buffer2, .len = (uint16_t)strlen(buffer2)};
    rstr      test3 = {.data = buffer3, .len = (uint16_t)strlen(buffer3)};
    rstr      test4 = {.data = buffer4, .len = (uint16_t)strlen(buffer4)};
    rstr      test5 = {.data = buffer5, .len = (uint16_t)strlen(buffer5)};

    auto      res1 = parser1.parse(test1);
    auto      res2 = parser2.parse(test2);
    auto      res3 = parser3.parse(test3);
    auto      res4 = parser4.parse(test4);
    auto      res5 = parser5.parse(test5);

    TEST_ASSERT_EQUAL(parser1.getCmpType(), SET);
    TEST_ASSERT_EQUAL(parser1.getState(), STOP);
    TEST_ASSERT_EQUAL_STRING(parser1.getFields().state, "STOP");

    TEST_ASSERT_EQUAL(parser2.getCmpType(), SET);
    TEST_ASSERT_EQUAL(parser2.getState(), READY);
    TEST_ASSERT_EQUAL_STRING(parser2.getFields().state, "READY");

    TEST_ASSERT_EQUAL(parser3.getCmpType(), SET);
    TEST_ASSERT_EQUAL(parser3.getState(), GO);
    TEST_ASSERT_EQUAL_STRING(parser3.getFields().state, "GO");

    TEST_ASSERT_EQUAL(parser4.getCmpType(), SET);
    TEST_ASSERT_EQUAL(parser4.getState(), CAUTION);
    TEST_ASSERT_EQUAL_STRING(parser4.getFields().state, "CAUTION");

    TEST_ASSERT_EQUAL(parser5.getCmpType(), SET);
    TEST_ASSERT_EQUAL(parser5.getState(), OFF);
    TEST_ASSERT_EQUAL_STRING(parser5.getFields().state, "OFF");
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_invalid_normal_tag);
    RUN_TEST(test_invalid_complicated_tag);
    RUN_TEST(test_buffer_overflow);
    RUN_TEST(test_empty_string);
    RUN_TEST(test_comment);
    RUN_TEST(test_bad_states);
    RUN_TEST(test_ping_pong);
    RUN_TEST(test_valid_states);

    UNITY_END();
}
