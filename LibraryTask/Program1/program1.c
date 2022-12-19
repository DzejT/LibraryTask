#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <logger.h>
#include <signal.h>
#define LOG_PATH "/var/log/log.db"


static int run_loop = 1;

static void signal_handler();
void generate_insertion(sqlite3 *db, int program_nr);

int main(){

    sqlite3 *db;
    int response = 1;
    struct sigaction sa = {0};
    sa.sa_handler = signal_handler;
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);

    response = open_log(&db, LOG_PATH);

    if(response == 0){
        while(run_loop == 1){
            generate_insertion(db, 1);
            sleep(15);
        }

        close_log(db);
    }
    else
        fprintf(stderr, "Unable to open database");
    
    

    return 0;
}

static void signal_handler()
{
    printf("Stopping program\n");
    run_loop = 0;
}

void generate_insertion(sqlite3 *db, int program_nr){
    char array_of_text[5][2][60] = {
        {"error", "Out of disk space"},
        {"warning", "possible memory leaks"},
        {"update", "database is being updated"},
        {"error", "reached maximum number of elements in database"},
        {"warning", "database is close to reaching max capacity"}
    };
    srand(time(NULL));   
    int r = rand()%5;

    insert_into_log(db, array_of_text[r][1], array_of_text[r][0], program_nr);
}
