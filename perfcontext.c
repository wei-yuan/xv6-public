#include "types.h"
#include "defs.h"

const char list_string[] = "list";
const char stat_string[] = "stat";
const char sched_string[] = "sched";
const char help_string[] = "help";
const char help_dash_string[] = "--help";
const char dash_string[] = "--";
const char hardware_event_string[] = "[Hardware event]";
const char software_event_string[] = "[Software event]";
const char Hardware_cache_event_stirng[] = "[Hardware cache event]";
const char list_event_string[] = "List of pre-defined events:";
const char usage_string[] = "usage: perf [FLAG] COMMAND [ARGS]";
const char common_string[] = "The most commonly used perf commands are:";
char not_implement_string[] = "perf: '%s' has not implemented yet\n";
char unknown_cmd_string[] = "perf: '%s' is not a perf-command. See 'perf --help'.\n";

struct map {
	const char *name;
	const char *descript;
};

struct map cmds[] = {
	{ list_string, "List all symbolic event types" },
	{ stat_string, "Run a command and gather performance counter statistics" },
  { sched_string, "Tool to trace/measure scheduler properties (latencies)" }
};

struct map list_items[] = {
 	{ "cpu-cycles OR cycles", hardware_event_string },
	{ "instructions", hardware_event_string },
  { "cache-misses", hardware_event_string },
  { "branch-misses", hardware_event_string},
  { "", ""},
  { "cpu-clock", software_event_string},
  { "page-faults OR faults", software_event_string},
  { "context-switches OR cs", software_event_string},
  { "", ""},  
  { "L1-dcache-loads", Hardware_cache_event_stirng},
  { "L1-dcache-load-misses", Hardware_cache_event_stirng},
  { "L1-dcache-stores", Hardware_cache_event_stirng},
  { "L1-dcache-store-misses", Hardware_cache_event_stirng},
  { "L1-dcache-prefetches", Hardware_cache_event_stirng},
  { "L1-dcache-prefetch-misses", Hardware_cache_event_stirng},
  { "L1-icache-loads", Hardware_cache_event_stirng},
  { "L1-icache-load-misses", Hardware_cache_event_stirng},
  { "L1-icache-prefetches", Hardware_cache_event_stirng},
  { "L1-icache-prefetch-misses", Hardware_cache_event_stirng}
};

void
perflist()
{
  unsigned int i, array_size;
  array_size = sizeof(list_items) / sizeof(struct map);
	cprintf("\n %s\n\n", list_event_string);
	for (i = 0; i < array_size; i++) {
		cprintf("   %s  %s\n", list_items[i].name, list_items[i].descript);
	}
  cprintf("\n");
}

void
perfstat()
{
  cprintf(not_implement_string, stat_string);
}

void
perfsched()
{
  cprintf(not_implement_string, sched_string);
}

void
perfhelp()
{
  unsigned int i, array_size;
  array_size = sizeof(cmds) / sizeof(struct map);
  cprintf("\n %s\n\n", usage_string);
  cprintf(" %s\n", common_string);
	for (i = 0; i < array_size; i++) {
		cprintf("   %s  %s\n", cmds[i].name, cmds[i].descript);
	}
  cprintf("\n");
}

void
perfcontext(char* flag)
{
  if (strncmp(flag, list_string, 4) == 0){
    perflist();
  } else if (strncmp(flag, stat_string, 4) == 0){
    perfstat();
  } else if(strncmp(flag, sched_string, 4) == 0){
    perfsched();
  } else if(strncmp(flag, help_string, 4) == 0 || strncmp(flag, help_dash_string, 4) == 0){
    perfhelp();
  } else {
    cprintf(unknown_cmd_string, flag);    
  }
}
