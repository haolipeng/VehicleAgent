#ifndef FLB_SQLDB_H
#define FLB_SQLDB_H

#include <sqlite3.h>
#include <fluent-bit.h>
#include <fluent-bit/flb_lock.h>

struct flb_sqldb {
    char *path;               /* physical path of the database */
    char *desc;               /* database description          */
    int shared;               /* is it a shared handler ?      */
    int users;                /* number of active users        */
    void *parent;             /* if shared, ref to parent      */
    sqlite3 *handler;         /* SQLite3 handler               */
    flb_lock_t lock;          /* thread safety mechanism       */
    struct mk_list _head;     /* Link to config->sqldb_list    */
};

struct flb_sqldb *flb_sqldb_open(const char *path, const char *desc,
                                 struct flb_config *config);
int flb_sqldb_close(struct flb_sqldb *db);

int flb_sqldb_query(struct flb_sqldb *db, const char *sql,
                    int (*callback) (void *, int, char **, char **),
                    void *data);

int64_t flb_sqldb_last_id(struct flb_sqldb *db);

int flb_sqldb_lock(struct flb_sqldb *db);

int flb_sqldb_unlock(struct flb_sqldb *db);

#endif
