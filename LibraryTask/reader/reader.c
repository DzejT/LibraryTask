#include <logger.h>
#include <sqlite3.h>

int main(int argc, char **argv){
    sqlite3 *db;

    open_log(&db);

    print_log(db, argc, argv);
    
    close_log(db);

    return 0;
}

