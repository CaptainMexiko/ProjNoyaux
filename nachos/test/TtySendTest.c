#include "userlib/syscall.h"
#include "userlib/libnachos.h"

int main() {
  //char * to_send = "Hello World";
  char * to_send = "Hello";
  n_printf("Début du TtySend()\n");
  TtySend(to_send);
  n_printf("Fin du TtySend()\n");
  return(0);
}
