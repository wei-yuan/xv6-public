struct perfdata {
    uint ticks;
	int conswch;
	int pgfault;
	int cpuswch;
};

struct perfcmd{
    char* cmd; //
    int arg1; // specific pid, if 0 -> system level
};

struct schedRecProc {
  int rec;
  int pid;                     // Process ID
  char name[16];               // Process name (debugging)
  int _tick;
  int startTicks;                 
  int endTicks;   
  int sumTicks;               
};