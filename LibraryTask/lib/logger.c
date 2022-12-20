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


int insert_into_log(sqlite3 *db, char *text, char *status, int program){
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

        return 1;         
    }
    return 0;

}

int print_log(sqlite3 *db, char *status, int programID){
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

    return sql_request(db, sql);
}


static int sql_request(sqlite3 *db, char *sql){
    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
        
    }

    return 0;
}


static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
        
    for (int i = 0; i < argc; i++) {

        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    
    printf("\n");
    
    return 0;
}


int open_log(sqlite3 **db, char *log_path){
    int rc = sqlite3_open(log_path, db);

    //if file not found create new file and try opening it again
    if(rc == SQLITE_CANTOPEN){
        int fd = creat(log_path, 0644);
        if(fd == -1){
            perror("creat");
            return 1;
        }    
        rc = sqlite3_open(log_path, db);
    }

    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(*db));
        sqlite3_close(*db);

        return 1;
    }

    return create_table(db);
}


static int create_table(sqlite3 **db){
    
    char *err_msg = 0;

    char* sql = "CREATE TABLE IF NOT EXISTS log (Status TEXT NOT NULL, Program INT NOT NULL, Text TEXT NOT NULL, Date DATETIME);";

    int rc = sqlite3_exec(*db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);        
        sqlite3_close(*db);
        
        return 1;
    }
    return 0;
}

int clear_log(sqlite3 *db){
    char *err_msg = 0;

    char* sql = "DELETE FROM log";

    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);        
        sqlite3_close(db);
        
        return 1;
    }
    return 0; 
}

int close_log(sqlite3 *db){
    int rc = sqlite3_close(db);
    if (rc != SQLITE_OK ) {
        sqlite3_close(db);
        return 1;
    }
    return rc;
}

