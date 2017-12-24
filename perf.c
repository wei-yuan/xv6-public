#include "types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    perf("help");
    exit();
  }

  for(i = 1; i < argc; i++){
    perf(argv[i]);
  }
  exit();
}