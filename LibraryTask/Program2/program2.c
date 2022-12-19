#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <logger.h>
#include <signal.h>
#define LOG_PATH "/var/log/log.db"


static int run_loop = 1;

static void signal_handler();


int main(){

    sqlite3 *db;

    struct sigaction sa = {0};
    sa.sa_handler = signal_handler;
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);

    open_log(&db, LOG_PATH);

    while(run_loop == 1){
        generate_insertion(db, 2);
        sleep(20);
    }

    close_log(db);

    return 0;
}

static void signal_handler()
{
    printf("Stopping program\n");
    run_loop = 0;
}

