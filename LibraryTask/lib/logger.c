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

// #define LOG_PATH "/var/log/log.db"


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

void print_log(sqlite3 *db, char *status, int programID){
    char sql[256] = "SELECT rowid, * FROM log";
    int index = 0;
    char temp[256];

    if(status != NULL){
        sprintf(temp, " WHERE Status ='%s'", status);
        strcat(sql, temp);
        index++;  
    }

    if(programID != 0){
        if(index == 0){
            sprintf(temp, " WHERE Program = %d", programID);
            strcat(sql, temp);
        }
        else{
            sprintf(temp, " AND Program = %d", programID);
            strcat(sql, temp);
        }
    }

    // printf("%s", sql);

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


void open_log(sqlite3 **db, char *log_path){
    int rc = sqlite3_open(log_path, db);

    //if file not found create new file and try opening it again
    if(rc == 14){
        int fd = creat(log_path, 0644);
        if(fd == -1){
            perror("creat");
            return;
        }    
        rc = sqlite3_open(log_path, db);
    }

    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(*db));
        sqlite3_close(*db);

        exit(1);
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
        
        exit(1);
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
