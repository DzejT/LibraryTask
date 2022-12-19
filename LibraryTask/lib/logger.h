#include <sqlite3.h>

int close_log(sqlite3 *db);
int insert_into_log(sqlite3 *db, char *text, char *status, int program);
int print_log(sqlite3 *db, char *status, int programID);
int open_log(sqlite3 **db, char *log_path);
static int create_table(sqlite3 **db);
static int callback(void *, int, char **, char **);
static int sql_request(sqlite3 *db, char *sql);
int clear_log(sqlite3 *db);
