#include "userlib/syscall.h"
#include "userlib/libnachos.h"


int main() {
  SemId sema = SemCreate((char *)"testCrea", 0);
  int errSema = SemDestroy(sema);
  if (errSema < 0) {
    PError("Erreur destruction sema : ");
    Exit(-1);
  }
  LockId lock = LockCreate((char *)"testCrea");
  int errLock = LockDestroy(lock);
  if (errLock < 0) {
    PError("Erreur destruction lock : ");
    Exit(-1);
  }
  CondId cond = CondCreate((char *)"testCrea");
  int errCond = CondDestroy(cond);
  if (errCond < 0) {
    PError("Erreur destruction cond : ");
    Exit(-1);
  }
  return 0;
}
