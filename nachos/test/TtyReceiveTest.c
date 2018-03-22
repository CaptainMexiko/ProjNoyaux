#include "userlib/syscall.h"
#include "userlib/libnachos.h"

void main() {
  int TAILLE_MAX = 11;
  char buff_target[TAILLE_MAX];
  char * to_recieve = buff_target;
  while (1) {
    TtyReceive(to_recieve, TAILLE_MAX);
    n_printf("%s\n", buff_target);
  }
}
