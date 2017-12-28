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

    printf(1, "%s %s %s",argv[0], argv[1], argv[2]);

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
          printf(1,"begin %d\n",cmd->arg1);
          perf_stat(cmd,st);
          exec(argv[2], &argv[2]);
        }
        
        wait();
        cmd->arg1 = pid;
        cmd->cmd = "end";
        perf_stat(cmd,ed);

        printf(1,"\nstart ticks: %d\n",st->ticks);
        printf(1,"start conswch: %d\n",st->conswch);
        printf(1,"start pgfault: %d\n",st->pgfault);
        printf(1,"\nend ticks: %d\n",ed->ticks);
        printf(1,"end conswch: %d\n",ed->conswch);
        printf(1,"end pgfault: %d\n\n",ed->pgfault);

        int ticks;
        int cxtsw;
        int cpusw;
        int pgfault;
        ticks = (ed->ticks - st->ticks);
        cxtsw = (ed->conswch - st->conswch);
        cpusw = ed->cpuswch - st->cpuswch;
        pgfault = ed->pgfault - st->pgfault;

        printf(1,"done! %d\n",pid);
        printf(1,"ticks: %d\n",ticks);
        printf(1,"context switch: %d\n",cxtsw);
        printf(1,"cpu switch: %d\n",cpusw);
        printf(1,"page fault: %d\n",pgfault);
    }

    exit();
}