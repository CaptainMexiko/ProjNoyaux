//A simple test to check the creation and behavior of the Lock implementation

#include "userlib/syscall.h"
#include "userlib/libnachos.h"

LockId lock;

VoidNoArgFunctionPtr compute(){
  LockAcquire(lock);
  int i;
  for (i = 0; i < 50; i++) {
    n_printf("Test des locks %d", i);
  }
  LockRelease(lock);
  return 0;
}

int main() {

  lock = LockCreate((char *)"Lock ");
  int i;
  for (i = 0; i < 2; i++) {
    threadCreate((char*)"process " + i, (VoidNoArgFunctionPtr) &compute);
  }

  return 0;
}
