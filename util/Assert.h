#ifndef ASSERT_H
#define ASSERT_H

/**
 * Prevent compilation if assertion is false.
 * Thanks to http://www.jaggersoft.com/pubs/CVu11_3.html
 */
#define Assert_UNIQUE_NAME Assert_MAKE_NAME(__LINE__)
#define Assert_MAKE_NAME(line) Assert_MAKE_NAME2(line)
#define Assert_MAKE_NAME2(line) Assert_testStruct_ ## line
#define Assert_assertTrue(isTrue) struct Assert_UNIQUE_NAME { unsigned int assertFailed : (isTrue); }

#endif
