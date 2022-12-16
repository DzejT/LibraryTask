#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "logger.h"
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



void insert_into_log(sqlite3 *db, char *text, char *status, int program){
    char sql[150];
    char *err_msg = 0;

    time_t mytime = time(NULL);
    char * time_str = ctime(&mytime);
    time_str[strlen(time_str)-1] = '\0';

    sprintf(sql, "INSERT INTO log (Status, Program, Text, Date) VALUES('%s', %d, '%s', '%s');", status, program, text, time_str);


    printf("%s\n", sql);   

    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);


    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);        
        sqlite3_close(db);

        exit(1);
        
    }

}

void print_log(sqlite3 *db, int argc, char **argv){
    char sql[120];
    int opt;
    int index = 0;
   

    while((opt = getopt(argc, argv, "12ewuc?")) != -1) 
    {
        switch(opt) 
        { 
            case '1':
                index == 0 ? strcpy(sql, "SELECT rowid, * FROM log WHERE Program = 1") : strcat(sql, " AND Program = 1");
                break;
            case '2':
                index == 0 ? strcpy(sql, "SELECT rowid, * FROM log WHERE Program = 2"): strcat(sql, " AND Program = 2");
                break;
            case 'e':
                index == 0 ? strcpy(sql, "SELECT rowid, * FROM log WHERE Status = 'error'") : strcat(sql, " AND Status = 'error'");
                break;
            case 'w':
                index == 0 ? strcpy(sql, "SELECT rowid, * FROM log WHERE Status = 'warning'") : strcat(sql, " AND Status = 'warning'");
                break;
            case 'u':
                index == 0 ? strcpy(sql, "SELECT rowid, * FROM log WHERE Status = 'update'") : strcat(sql, " AND Status = 'update'");
                break;
            case 'c':
                clear_log(db);
                sqlite3_close(db);
                exit(1);
                break;
            case '?':
                fprintf(stderr, "Unknown option or missing an argument \n");
                printHelp();
                sqlite3_close(db);
                exit(1);
            default:
                printf("Incorrect argument %c \n", opt);
                printHelp();
                sqlite3_close(db);
                exit(1);

        }
        index++;
    }

    if(index != 0){
        sql_request(db, sql);
        return;
    }

    strcpy(sql, "SELECT rowid, * FROM log");
    sql_request(db, sql);

}

void sql_request(sqlite3 *db, char *sql){
    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(1);
        
    }
}


int callback(void *NotUsed, int argc, char **argv, char **azColName) {
        
    for (int i = 0; i < argc; i++) {

        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    
    printf("\n");
    
    return 0;
}


void open_log(sqlite3 **db){
    int rc = sqlite3_open("/var/log/log.db", db);

    //if file not found create new file and try opening it again
    if(rc == 14){
        int fd = creat("/var/log/log.db", 0644);
        if(fd == -1){
            perror("creat");
            return;
        }    
        rc = sqlite3_open("/var/log/log.db", db);
    }

    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(*db));
        sqlite3_close(*db);

        return;
    }

    create_table(db);
    
}


void create_table(sqlite3 **db){
    
    char *err_msg = 0;

    char* sql = "CREATE TABLE IF NOT EXISTS log (Status TEXT NOT NULL, Program INT NOT NULL, Text TEXT NOT NULL, Date DATETIME);";

    int rc = sqlite3_exec(*db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);        
        sqlite3_close(*db);
        
        return;
    } 
}

void clear_log(sqlite3 *db){
    char *err_msg = 0;

    char* sql = "DELETE FROM log";

    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);        
        sqlite3_close(db);
        
        return;
    } 
}

void close_log(sqlite3 *db){
    sqlite3_close(db);
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

void printHelp(){
    printf("Possible arguments:\n");
    printf("-1 prints log from first program\n");
    printf("-2 prints log from second program\n");
    printf("-e prints all the errors\n");
    printf("-w prints all the warnings\n");
    printf("-u prints all the updates\n");
    printf("-c clears the log\n");
    printf("argumets can be combined\n");
    printf("for example: ./reader -1 -e prints all errors from first program\n")
}