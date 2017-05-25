#include <time.h>
#include <stdio.h>
#ifndef _file_handling_h
#define _file_handling_h


/*  SPEC POUR LA DATABASE
 * 1. possibilités de plusieurs databases
 * 2. Uniformité des requêtes (ID, type opération,montant,date)
 * 3. Plusieurs comptes -> plusieurs databases
 * 4. Fichiers -> différents accompte de bank???
 * 5. 
 *
*/
typedef enum DataType {
    MONEY,
    TIME,
    OPERATION_TYPE
} Data_type;

typedef union DataTypeUnion {
    unsigned int i;
    time_t t;
    char* ucptr;
} Data_type_union;
/*
 * Column of database
 */
struct Operation {
  unsigned int ID;
  unsigned int is_set;
  char* operation_type;
  unsigned int money;
  time_t time;
};

/*
 * Rows of database
 */
struct Database {
    int max_data;
    int max_rows;
    struct Operation* rows; 
};

/*
 * File descriptor for database and pointer to memory pool of database.
*/
struct Connection {
    FILE *file; 
    struct Database *db;
};

/*
 * Create database from a name given or open the existing one and alloc mem for data pool.
 * Return a file descriptor. 
 */
struct Connection* open_database(const char* filename, int max_data, int max_rows);

/*
 * Open database file and populate the datas in mem
 * Return error code 
 */
int load_database(struct Connection* conn);


void read_database(struct Connection* conn); 

/*
 * Populate database with default info.
 * Return error code.
 */ 
void populate_database(struct Connection* conn);

/*
 * Close database and dealocate mem 
 * Return sucess or fail.
 */
void close_database(struct Connection* conn);

/*
 * Write new datas to mem then write in the file descriptor.
 * return sucess or fail
 */
int write_database(struct Connection* conn);


/*
 * set value in mem then it will need to be written to file. 
 * return sucess or fail
 */
void set_database(struct Connection* conn, int id, int amount_money, const char* operation_done); 

/*
 * get value in mem and return it
 * return value
 */
struct Operation* get_database(struct Connection* conn, Data_type_union data, Data_type dataType);
    
/*
 * Display all the database from file descriptor 
 * Return Sucess or fail.
 * Calls read_database recursively until it reaches NULL?
 */
void display_database(struct Connection* conn);

/*
 * reset database to default value
 * return sucess or fail.
 */
void reset_database(struct Connection* conn);

/*
 * Delete database file but not memory
 * return sucess or fail.
 */
void delete_database(struct Connection* conn);
#endif
