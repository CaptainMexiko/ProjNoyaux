#include "userlib/syscall.h"
#include "userlib/libnachos.h"

int main() {
  int TAILLE_MAX = 11;
  char buff_target[TAILLE_MAX];
  char * to_recieve = buff_target;
  TtyReceive(to_recieve, TAILLE_MAX);
  n_printf("%s\n", buff_target);
  return(0);
}
