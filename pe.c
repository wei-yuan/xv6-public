#include "types.h"
#include "stat.h"
#include "user.h"
#include "perf.h"

int
countdigit(int n)
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

char* 
calms(int n)
{
  const char zero_point[] = "0.";
  const char zero[] = "0";
  char *result;
  result = "";
  result = strcat(result, zero_point);

  int i;
  // one million has six zero digit
  int zero_num = 6 - countdigit(n);
  for (i = 0; i < zero_num; i++) {
    result = strcat(result, zero);  
  }
  return result;
}

int main(int argc, char *argv[]){

    //perf stat ls
    if(argc <= 1){
        printf(2,"few arguments\n");
        exit();
    }

    struct perfcmd *cmd ;
    struct perfdata *st, *ed ;

    cmd = malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));

    st = malloc(sizeof(*st));
    memset(st, 0, sizeof(*st));

    ed = malloc(sizeof(*ed));
    memset(ed, 0, sizeof(*ed));

    //pe stat
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
          perf_stat(cmd,st);
          exec(argv[2], &argv[2]);
        }
        
        wait();
        cmd->test_cmd = argv[2];
        cmd->arg1 = pid;
        cmd->cmd = "end";
        perf_stat(cmd,ed);


        printf(1,"instruction: %s\n",cmd->test_cmd);
        printf(1,"total ticks: %d\n",ed->totalticks);
        printf(1,"cpu ticks: %d\n",ed->cputicks);
        printf(1,"conswch: %d\n",ed->conswch);
        printf(1,"pgfault: %d\n",ed->pgfault);
        printf(1,"cpuswch: %d\n\n",ed->cpuswch);

    }

    exit();
}