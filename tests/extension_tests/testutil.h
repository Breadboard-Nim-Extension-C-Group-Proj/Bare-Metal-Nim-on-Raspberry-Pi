#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <stdbool.h>

void testcond(bool ok, char* testname);

// similar to kotlin iterator on chars, ignores whitespaces.
int text_file_get_next(FILE* f);

// compares whether 2 text files contain the same text, ignoring whitespaces.
int file_compare(char* fname1, char* fname2);
#endif
