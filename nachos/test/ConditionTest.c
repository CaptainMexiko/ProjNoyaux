#include "userlib/syscall.h"
#include "userlib/libnachos.h"

CondId cond;

VoidNoArgFunctionPtr worker(){
  n_printf("Work to be done");
  CondSignal(cond);
}

VoidNoArgFunctionPtr testSignal(){
  CondWait(cond);
  n_printf("Test de la condition");
}

int main() {

  cond = CondCreate((char *) "condTest");

  threadCreate((char *)"cond1", (VoidNoArgFunctionPtr)&testSignal);
  threadCreate((char *)"cond2", (VoidNoArgFunctionPtr)&worker);


  return 0;
}
