#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "testutil.h"


void testcond(bool ok, char *testname){
  printf("T %s: %s\n", testname, ok? "OK" : "FAIL");
}

int text_file_get_next(FILE* f) {
  int c;
  do {
    c = fgetc(f);
  } while (c != EOF && isspace((unsigned char) c));

  return c;
}

int file_compare(char* fname1, char* fname2) {
  FILE* f1 = fopen(fname1, "r");
  FILE* f2 = fopen(fname2, "r");

  if (!f1 || !f2) {
    return -1; // error since file not found
  }

  int c1, c2;
  do {
    c1 = text_file_get_next(f1);
    c2 = text_file_get_next(f2);
    // printf("%d %d \n",c1, c2);
    if (c1 != c2) {
      fclose(f1);
      fclose(f2);
      return 0; // i.e. false
    }
    
  } while (c1 != EOF && c2 != EOF);
  
  fclose(f1);
  fclose(f2);
  return 1; // i.e. true
}
