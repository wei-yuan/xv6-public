struct perfdata {
    uint totalticks; //including sleeping and wait..
    uint cputicks;
	int conswch;
	int pgfault;
	int cpuswch;
};

struct perfcmd{
    char* test_cmd;
    char* cmd; //
    int arg1; // specific pid, if 0 -> system level
};

struct perf_record{
    int recording;
    int pid;
    int cxtsw;
    int pgfault;
    int _ticks;
    int sumticks;
    int startticks;
    int endticks;
    int cpuid;
    int cpusw;
};

// for any string pair structure
struct perfoutput {
    const char* title;
    const char* descript; // description
};