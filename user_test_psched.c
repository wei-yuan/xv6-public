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
        printf(1, "Test system call psched success: %d", err);
    } 
    else
    {
        printf(1, "Test system call psched fail: %d\n", err);
    }
        
    exit();
}