//A simple program to test the behavior of Semaphor in a Producteur/Consomateur setting (use keyboard interruption to see the interactions between processus)

#include "userlib/syscall.h"
#include "userlib/libnachos.h"

SemId bufferPlein;
SemId bufferVide;
char buffer[16];

VoidNoArgFunctionPtr remplirBuffer(){
  while (1) {
    P(bufferVide);
    n_printf("Début du remplissage");
    int i;
    for (i = 0; i <= 16; i++) {
      n_printf("Remplissage en cours");
      buffer[i] = 'a';
    }
    n_printf("Remplissage finis");
    V(bufferPlein);
  }
}

VoidNoArgFunctionPtr consommerBuffer(){
  while (1) {
    P(bufferPlein);
    n_printf("Début de la consomation");
    n_memset(&buffer, (int)"\0", 16);
    n_printf("Consomation terminé");
    V(bufferVide);
  }
}

int main() {
  n_printf("Debut du programme de test");

 bufferPlein = SemCreate((char *)"bufferPlein", 0);
 bufferVide = SemCreate((char *)"bufferVide", 1);

  threadCreate((char *)"producteur", (VoidNoArgFunctionPtr)&remplirBuffer);
  threadCreate((char *)"consomateur", (VoidNoArgFunctionPtr)&consommerBuffer);

  return 0;
}
