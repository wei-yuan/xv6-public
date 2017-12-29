#include "types.h"
#include "stat.h"
#include "user.h"
#include "perf.h"

int
count_digit(int n)
{
  int f = n;
  int digit = 1;
  for (;;) {
    f /= 10;
    if (f == 0)
      break;
    digit++;
  }
  return digit;
}

void
printfms(char *s, int n)
{
  const char point[] = ".";
  const char zero[] = "0";
  char result[] = "\0";
  
  if (n != 0) {
    // one million has six zero digit
    int zero_num = 6 - count_digit(n);
    int i;
    strcat(result, point);        
    for (i = 0; i < zero_num; i++) {
      strcat(result, zero);    
    }
    printf(1, "  %d %s#      %d%s%d M/sec\n", n, s, n/1000000, result, n);
  } else {
    printf(1, "  0  %s#      0 M/sec\n", s);
  }
}

int main(int argc, char *argv[]){

    //perf stat ls
    if(argc <= 1){
        printf(2,"few arguments\n");
        exit();
    }

    struct perfcmd *cmd ;
    struct perfdata *ed ;

    cmd = malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));

    ed = malloc(sizeof(*ed));
    memset(ed, 0, sizeof(*ed));

    //perf stat
    if(strcmp(argv[1],"stat") == 0){

        if(argc < 3){
            printf(2,"few arguments\n");
            exit();
        }

        //hard code
        cmd->cmd = "stat";

        int pid;
        pid = fork();

        if(pid == 0){
          cmd->cmd = "start";
          cmd->arg1 = getpid();
          perf_stat(cmd,0);
          exec(argv[2], &argv[2]);
        }
        
        wait();
        cmd->test_cmd = argv[2];
        cmd->arg1 = pid;
        cmd->cmd = "end";
        perf_stat(cmd,ed);

        printf(1, "\nPerformance counter stats for '%s':\n\n", cmd->test_cmd);
        printfms("page-faults               ", ed->pgfault);
        printfms("context-switches          ", ed->conswch);
        printfms("cpu-ticks                 ", ed->cputicks);
        printfms("cpu-migrations            ", ed->cpuswch);        
        printf(1, "\n");
    }
    exit();
}