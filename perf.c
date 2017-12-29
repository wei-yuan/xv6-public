#include "types.h"
#include "stat.h"
#include "user.h"
#include "perf.h"

const char hardware_event[] = "[Hardware event]";
const char software_event[] = "[Software event]";
const char Hardware_cache_event[] = "[Hardware cache event]";
const char list_event_message[] = "List of pre-defined events:";
const char usage_message[] = "usage: perf [FLAG] [COMMAND] [ARGS]";
const char common_message[] = "The most commonly used perf commands are:";
struct perfoutput help_output[] = {
    { "list            ", "List all symbolic event types" },
    { "stat            ", "Run a command and gather performance counter statistics" },
    { "sched           ", "Tool to trace/measure scheduler properties (latencies)" }
};
struct perfoutput list_output[] = {
  { "cpu-migrations OR migrations               ", software_event },    
  { "cpu-ticks                                  ", software_event },  
  { "page-faults OR faults                      ", software_event },
  { "context-switches OR cs                     ", software_event }
};

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

void
perf_help(void)
{
  int i, array_size;
  array_size = sizeof(help_output) / sizeof(struct perfoutput);
  printf(1, "\n %s\n\n", usage_message);
  printf(1, " %s\n", common_message);
  for (i = 0; i < array_size; i++) {
    printf(1, "   %s%s\n", help_output[i].title, help_output[i].descript);
  }
  printf(1, "\n");
}

void
perf_list(void)
{
  int i, array_size;
  array_size = sizeof(list_output) / sizeof(struct perfoutput);
  printf(1, "\n %s\n\n", list_event_message);
  for (i = 0; i < array_size; i++) {
    printf(1, "   %s%s\n", list_output[i].title, list_output[i].descript);
  }
  printf(1, "\n");
}

int main(int argc, char *argv[]){

    if(argc <= 1){
        printf(2,"few arguments\n");
        exit();
    }

    //perf help
    if(strcmp(argv[1],"help") == 0){
        perf_help();
        exit();
    }

    //perf list 
    if(strcmp(argv[1],"list") == 0){
        perf_list();
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