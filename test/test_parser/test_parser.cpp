#include "../lib/XMLParser/XMLParser.h"
#include "../lib/Rstr/Rstr.h"
#include <cstdint>
#include <unity.h>

void test_not_self_closing_tag() {
    XMLParser parser      = XMLParser();
    char      buffer[256] = "<name>John</name>";
    rstr      test        = {.data = buffer, .len = (uint16_t)strlen(buffer)};

    int8_t    res = parser.set_buffer(test);
    TEST_ASSERT_EQUAL(1, res);
}

void test_complicated_tag() {
    XMLParser parser = XMLParser();
    char      buffer[256] =
        "<dependency><groupId>org.springframework.boot</groupId>"
        "<artifactId>spring-boot-starter</artifactId></dependency>";
    rstr   test = {.data = buffer, .len = (uint16_t)strlen(buffer)};

    int8_t res = parser.set_buffer(test);
    TEST_ASSERT_EQUAL(1, res);
}

void test_buffer_overflow() {
    XMLParser parser = XMLParser();
    char      buffer[512] =
        "<dependencies><dependency>"
        "<groupId>org.springframework.boot</groupId>"
        "<artifactId>spring-boot-starter</artifactId></dependency>"
        "<dependency><groupId>org.springframework.boot</groupId>"
        "<artifactId>spring-boot-starter-test</artifactId><scope>test</scope>"
        "</dependency><dependency>"
        "<groupId>org.springframework.boot</groupId>"
        "<artifactId>spring-boot-devtools</artifactId><scope>runtime</scope>"
        "</dependency>";
    rstr   test = {.data = buffer, .len = (uint16_t)strlen(buffer)};

    int8_t res = parser.set_buffer(test);
    TEST_ASSERT_EQUAL(1, res);
}

void test_empty_string() {
    XMLParser parser    = XMLParser();
    char      buffer[1] = "";
    rstr      test      = {.data = buffer, .len = (uint16_t)strlen(buffer)};

    int8_t    res = parser.set_buffer(test);
    TEST_ASSERT_EQUAL(0, res);
}

void test_comment() {
    XMLParser parser     = XMLParser();
    char      buffer[32] = "<!--this is a comment-->";
    rstr      test       = {.data = buffer, .len = (uint16_t)strlen(buffer)};

    auto      res = parser.is_comment(test);
    TEST_ASSERT_EQUAL(1, res);
}

void test_attribute_getter() {
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

    str_view  test_view1 = DEFAULT_RET;
    str_view  test_view2 = DEFAULT_RET;
    str_view  test_view3 = DEFAULT_RET;
    str_view  test_view4 = DEFAULT_RET;
    str_view  test_view5 = DEFAULT_RET;

    (void)parser.set_buffer(test1);
    auto res1 = parser.get_attribute("state", test_view1);
    TEST_ASSERT_EQUAL_STRING_LEN("SUPER", test_view1.data, test_view1.len);

    (void)parser.set_buffer(test2);
    auto res2 = parser.get_attribute("state", test_view2);
    TEST_ASSERT_EQUAL_STRING_LEN("ARDUINO", test_view2.data, test_view2.len);

    (void)parser.set_buffer(test3);
    auto res3 = parser.get_attribute("state", test_view3);
    TEST_ASSERT_EQUAL_STRING_LEN("COMPILER", test_view3.data, test_view3.len);

    (void)parser.set_buffer(test4);
    auto res4 = parser.get_attribute("state", test_view4);
    TEST_ASSERT_EQUAL_STRING_LEN("PARSER", test_view4.data, test_view4.len);

    (void)parser.set_buffer(test5);
    auto res5 = parser.get_attribute("state", test_view5);
    TEST_ASSERT_EQUAL_STRING_LEN("ARM", test_view5.data, test_view5.len);
}

void test_other_types_of_tags() {
    XMLParser parser = XMLParser();

    char      buffer1[35] = "<a href=\"http://localhost:3000\"/>";
    rstr     test_buffer1 = {.data = buffer1, .len = (uint16_t)strlen(buffer1)};
    str_view test_view1   = DEFAULT_RET;

    (void)parser.set_buffer(test_buffer1);
    auto res1 = parser.get_attribute("href", test_view1);
    TEST_ASSERT_EQUAL_STRING_LEN("http://localhost:3000", test_view1.data,
                                 test_view1.len);
}

void test_multiple_attributes() {
    XMLParser parser = XMLParser();

    char buffer[] = "<img src=\"image.png\" alt=\"An image\" width=\"500\"/>";
    rstr test_buffer = {.data = buffer, .len = (uint16_t)strlen(buffer)};
    str_view view    = DEFAULT_RET;

    (void)parser.set_buffer(test_buffer);
    auto res = parser.get_attribute("alt", view);
    TEST_ASSERT_EQUAL_STRING_LEN("An image", view.data, view.len);
}

void test_attribute_with_single_quotes() {
    XMLParser parser = XMLParser();

    char      buffer[]    = "<a href='https://example.com'/>";
    rstr      test_buffer = {.data = buffer, .len = (uint16_t)strlen(buffer)};
    str_view  view        = DEFAULT_RET;

    (void)parser.set_buffer(test_buffer);
    auto res = parser.get_attribute("href", view);
    TEST_ASSERT_EQUAL_STRING_LEN("https://example.com", view.data, view.len);
}

void test_missing_attribute() {
    XMLParser parser = XMLParser();

    char      buffer[]    = "<tag foo=\"bar\"/>";
    rstr      test_buffer = {.data = buffer, .len = (uint16_t)strlen(buffer)};
    str_view  view        = DEFAULT_RET;

    (void)parser.set_buffer(test_buffer);
    auto res = parser.get_attribute("baz", view);
    TEST_ASSERT_EQUAL(0, view.len);
}

void test_attribute_with_special_characters() {
    XMLParser parser = XMLParser();

    char      buffer[]    = "<tag data=\"a&b<c>d\"/>";
    rstr      test_buffer = {.data = buffer, .len = (uint16_t)strlen(buffer)};
    str_view  view        = DEFAULT_RET;

    (void)parser.set_buffer(test_buffer);
    auto res = parser.get_attribute("data", view);
    TEST_ASSERT_EQUAL_STRING_LEN("a&b<c>d", view.data, view.len);
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_not_self_closing_tag);
    RUN_TEST(test_complicated_tag);
    RUN_TEST(test_buffer_overflow);
    RUN_TEST(test_empty_string);
    RUN_TEST(test_comment);
    RUN_TEST(test_attribute_getter);
    RUN_TEST(test_other_types_of_tags);
    RUN_TEST(test_multiple_attributes);
    RUN_TEST(test_attribute_with_single_quotes);
    RUN_TEST(test_missing_attribute);
    RUN_TEST(test_attribute_with_special_characters);

    UNITY_END();
}
