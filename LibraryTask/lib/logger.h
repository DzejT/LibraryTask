#include <sqlite3.h>

void close_log(sqlite3 *db);
void insert_into_log(sqlite3 *db, char *text, char *status, int program);
void print_log(sqlite3 *db, char *status, int programID);
void open_log(sqlite3 **db, char *log_path);
void create_table(sqlite3 **db);
int callback(void *, int, char **, char **);
void sql_request(sqlite3 *db, char *sql);
void generate_insertion(sqlite3 *db, int program_nr);
void clear_log(sqlite3 *db);
