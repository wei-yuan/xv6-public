#include "types.h"
#include "stat.h"
#include "user.h"

int 
main(void)
{
    int err = 0;

    err = psched();

    if(err == 1)
    {
        //Do nothing
    } 
    else
    {
        printf(1, "\nTest system call psched fail: %d\n", err);
    }
        
    exit();
}