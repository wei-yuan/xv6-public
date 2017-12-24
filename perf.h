struct perfdata {
    uint ticks;
	int conswch;
	int pgfault;
	int cpuswch;
};

struct perfcmd{
    char* cmd;
    int arg1;
};