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