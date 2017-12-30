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
printf_sperm(char *s, int n, int mticks)
{
  const char point[] = ".";
  const char zero[] = "0";
  char result[] = "\0";

  // should check overflow
  int mmticks = mticks * 1000;
  int in = n/mmticks;
  
  if (n == 0){
    printf(1, "  0  %s#      0 M/sec\n", s);
  } else if (in > 0) {
    printf(1, "  %d %s#      %d M/sec\n", n, s, in);            
  } else {
    int cn = n;
    int digit = 1;
    for(;;) {
      cn *= 10;
      digit++;
      if ((cn/mticks)/1000 > 0) {
        cn *= 10;
        digit++;
        break;
      }
    }
    int zero_num = digit - 3;
    int i;
    strcat(result, point); 
    for (i = 0; i < zero_num; i++) {
      strcat(result, zero);    
    }
    printf(1, "  %d %s#      %d%s%d M/sec\n", n, s, 0, result, (cn/mticks)/1000);        
  }
}

void
printf_m2s(char* s, int m)
{
  const char point[] = ".";
  const char zero[] = "0";
  char result[] = "\0";
  
  if (m%1000 > 0) {
    int tm = m - ((m/1000) * 1000);
    int zero_num = 3 - count_digit(tm);
    int i;
    strcat(result, point);        
    for (i = 0; i < zero_num; i++) {
      strcat(result, zero);    
    }
    printf(1, "\n %d%s%d %s\n\n", m/1000, result, tm, s);    
  } else {
    printf(1, "\n %d %s\n\n", m/1000, s);    
  }
}

void
printf_cpus(char* s, int mcticks, int mtticks)
{
  printf(1, "  %d %s#      %d CPUs utilized\n", mcticks, s, mcticks/mtticks);
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
     
        // ticks vs seconds?     
        int mcputicks = ed->cputicks * 10;
        int mtotalticks= ed->totalticks * 10;

        // CPUs utilized = cpu-ticks msec / msec time elapsed
        // M/sec = item / cpu-ticks msec / 1000

        printf(1, "\nPerformance counter stats for '%s':\n\n", cmd->test_cmd);
        printf_sperm("    page-faults               ", ed->pgfault, mtotalticks);
        printf_sperm("    context-switches          ", ed->conswch, mtotalticks);
        printf_cpus("   cpu-ticks msec            ", mcputicks, mtotalticks);
        printf_sperm("    cpu-migrations            ", ed->cpuswch, mtotalticks); 
        printf_m2s("seconds time elapsed", mtotalticks);
    }
    exit();
}