#include "types.h"
#include "user.h"


int
main(int argc, char *argv[]){

    if(argc < 1){
        printf(2,"few arguments\n");
        exit();
    }

    int period;
    period = atoi(argv[1]);

    sleep(period);

    exit();

}