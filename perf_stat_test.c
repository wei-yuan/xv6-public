#include "types.h"
#include "user.h"
#include "stat.h"

int main()
{
    printf(1, "Process Scheduling statistics\n");
    printf(1, "tick\tInterval\n");

    struct pstat st;
    perf();

    for(int i=0; i<NPROC; i++)
    {

    }

    exit();
}