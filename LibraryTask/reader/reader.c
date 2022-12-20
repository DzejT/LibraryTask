#include <logger.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LOG_PATH "/var/log/log.db"


void printHelp();
void parse_argumets(sqlite3 *db, int argc, char **argv);




int main(int argc, char **argv){
    sqlite3 *db = NULL; 

    open_log(&db, LOG_PATH);

    if(db == NULL){
        printf("Unable to open log\n");
        close_log(db);
        exit(1);
    }

    parse_argumets(db, argc, argv);

    close_log(db);

    return 0;
}


void printHelp(){
    printf("Possible arguments:\n");
    printf("-1 prints log from first program\n");
    printf("-2 prints log from second program\n");
    printf("-e prints all the errors\n");
    printf("-w prints all the warnings\n");
    printf("-u prints all the updates\n");
    printf("-c clears the log\n");
    printf("argumets can be combined\n");
    printf("for example: ./reader -1 -e prints all errors from first program\n");
}

void parse_argumets(sqlite3 *db, int argc, char **argv){
    char *status = NULL;
    int programID = 0;


    int opt;

    while((opt = getopt(argc, argv, "12ewuc?")) != -1) 
    {
        switch(opt) 
        { 
            case '1':
                programID = 1;
                break;
            case '2':
                programID = 2;
                break;
            case 'e':
                status = "error";
                break;
            case 'w':
                status = "warning";
                break;
            case 'u':
                status = "update";
                break;
            case 'c':
                clear_log(db);
                close_log(db);
                exit(1);
                break;
            case '?':
                fprintf(stderr, "Unknown option or missing an argument \n");
                printHelp();
                close_log(db);
                exit(1);
            default:
                printf("Incorrect argument %c \n", opt);
                printHelp();
                close_log(db);
                exit(1);

        }
    }
    print_log(db, status, programID);
}