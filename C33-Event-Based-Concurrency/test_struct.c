#include <stdlib.h>
#include <stdio.h>

typedef struct _foo {
  int val;
} foo;

int main(int argc, char *argv[]) {
  foo f1, f2;
  f1.val = 1;
  f2 = f1;
  f2.val = 2;
  printf("f1 val: %d\n", f1.val);
  printf("f2 val: %d\n", f2.val);
  
  return 0;
}
