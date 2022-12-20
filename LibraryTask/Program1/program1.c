#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <logger.h>
#include <signal.h>
#define LOG_PATH "/var/log/log.db"


static int run_loop = 1;

static void signal_handler();
static int generate_insertion(sqlite3 *db);
static void print_help();


int main(){

    sqlite3 *db;

    struct sigaction sa = {0};
    sa.sa_handler = signal_handler;
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);

    int rc;
    open_log(&db, LOG_PATH);
    
    while(run_loop == 1){
        if(db != NULL){
            rc = generate_insertion(db);
            printf("%d\n",rc);
            if(rc == 1){
                printf("if getting error: (SQL error: attempt to write a readonly database) try run program with sudo");
                run_loop = 0;
            }
            printf("here\n");
            sleep(5);
        }
        else{
            fprintf(stderr, "Unable to open the log\n");
            print_help();
            char c [2] = {0};
            fgets(c, sizeof(c), stdin);
            switch (c[0])
            {
            case 'r':
                printf("retrying to open log\n");
                open_log(&db, LOG_PATH);
                break;
            case 'e':
                printf("Exiting program\n");
                run_loop = 0;
                break;
            default:
                print_help();
                fprintf(stderr, "Unknown command\n");
                break;
            }
            
        }
    }
    
    close_log(db);

    return 0;
}

static void signal_handler()
{
    printf("Stopping program\n");
    run_loop = 0;
}

static int generate_insertion(sqlite3 *db){
    char array_of_text[5][2][60] = {
        {"error", "Out of disk space"},
        {"warning", "possible memory leaks"},
        {"update", "database is being updated"},
        {"error", "reached maximum number of elements in database"},
        {"warning", "database is close to reaching max capacity"}
    };
    srand(time(NULL));   
    int r = rand()%5;

    return insert_into_log(db, array_of_text[r][1], array_of_text[r][0], 1);
}

static void print_help(){
    printf("e - exit program\n");
    printf("r - retry to open the log\n");
}


