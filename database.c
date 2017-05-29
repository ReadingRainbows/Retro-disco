#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "database.h"
#include "fat_pointers.h"


/*
 * Create database from a name given or open the existing one and alloc mem for data pool.
 * Return a file descriptor. 
 */
struct Connection* open_database(const char* filename, int max_data, int max_rows) {

    struct Connection *conn = malloc(sizeof(struct Connection)); 
    if(!conn)
	exit(1);

    conn->db = malloc(sizeof(struct Database));
    if(!conn->db)
	exit(1);

    conn->db->max_data = max_data;
    conn->db->max_rows = max_rows;
    
    if(0 == access(filename, O_RDWR)) {
       conn->file = fopen(filename, "r+"); //File exist
    }
    else {
	conn->file = fopen(filename, "w"); //File doesn't exist
    }
	
    if(!conn->file)
	exit(1);
   
    populate_database(conn);
    return conn;
}

/*
 * Open database file and populate the datas in mem
 * Return error code 
 */
void read_database(struct Connection* conn) {
    int rc = 0;
    int i = 0;
    rc = fread(&conn->db->max_data, sizeof(int), 1, conn->file);
    rc = fread(&conn->db->max_rows, sizeof(int), 1, conn->file);

    for(i = 0; i < conn->db->max_rows; ++i) {
	rc = 0;
	rc += fread(&conn->db->rows[i].ID, sizeof(unsigned int), 1, conn->file);
	rc += fread(&conn->db->rows[i].is_set, sizeof(unsigned int), 1, conn->file);
	rc += fread(conn->db->rows[i].operation_type, sizeof(char) * conn->db->max_data, 1, conn->file);
	rc += fread(&conn->db->rows[i].money, sizeof(unsigned int), 1, conn->file);
	rc += fread(&conn->db->rows[i].time, sizeof(time_t), 1, conn->file);
	if(rc != 5) {
	    // exit(1);
	}
    } 
}

/*
 * Populate newly created database with default info in mem.
 * Return error code.
 */ 
void populate_database(struct Connection* conn) {
    int i = 0;

//    struct Operation* db_rows = malloc(sizeof(struct Operation)*conn->db->max_rows);
    conn->db->rows = malloc(sizeof(struct Operation)*conn->db->max_rows);
    memset(conn->db->rows, 0, sizeof(struct Operation)*conn->db->max_rows);

    for(i = 0; i < conn->db->max_rows; i++) {
	struct Operation operation = {.ID = i, .is_set = 0};
	
	memcpy(&conn->db->rows[i], &operation, sizeof(struct Operation));
	conn->db->rows[i].operation_type = malloc(sizeof(char) * conn->db->max_data);
	memset(conn->db->rows[i].operation_type, 0, sizeof(char) * conn->db->max_data);
    }
}



void close_database(struct Connection* conn) {
    int i = 0;

    if(conn) {
	if(conn->db) {
	    if(conn->db->rows) {
		for(i = 0; i < conn->db->max_rows; ++i) {
		    free(conn->db->rows[i].operation_type);
		    conn->db->rows[i].operation_type = NULL;
		}
	    }
	    free(conn->db->rows);
	    conn->db->rows = NULL;
	    free(conn->db);
	    conn->db = NULL;
	}
	fclose(conn->file);
	free(conn);
	conn = NULL;
    }
}
/*
 * Write new datas to mem then write in the file descriptor.
 * return sucess or fail
 */
int write_database(struct Connection* conn) {
    int wc = 0;
    int i = 0;
    int ret = 0;

    rewind(conn->file); //return file position to 0.

    //write column and rows in file to be sure to read it correctly after
    wc += fwrite(&conn->db->max_data, sizeof(int), 1, conn->file);
    wc += fwrite(&conn->db->max_rows, sizeof(int), 1, conn->file);

    //right every single case "Operation" in the database
    for(i = 0; i < conn->db->max_rows; ++i) {
	wc = 0;
	wc += fwrite(&conn->db->rows[i].ID, sizeof(unsigned int), 1, conn->file);
	wc += fwrite(&conn->db->rows[i].is_set, sizeof(unsigned int), 1, conn->file);
	wc += fwrite(conn->db->rows[i].operation_type, sizeof(char)*conn->db->max_data, 1, conn->file);
	wc += fwrite(&conn->db->rows[i].money, sizeof(int), 1, conn->file);
	wc += fwrite(&conn->db->rows[i].time, sizeof(time_t), 1, conn->file);
    }

    //error handling for wc!

    ret = fflush(conn->file);
    if(ret != 0)
	exit(1);


    return 0;
}


/*
 * set value in mem then it will need to be written to file. 
 * return sucess or fail
 */
void set_database(struct Connection* conn, int id, int amount_money, const char* operation_done) {

    struct Operation* operation = &conn->db->rows[id];

    if(operation->is_set)
	return;
    operation->is_set = 1;

    char* res = strncpy(operation->operation_type, operation_done, conn->db->max_data);
    if(!res)
	exit(1);


    operation->money = amount_money;

    operation->time = time(NULL);
    // set in mem values.
}

/*
 * get value in mem and return it
 * return valuelinkedin
 */
struct Operation* get_database(struct Connection* conn, Data_type_union data, Data_type dataType) {
    int i = 0;
    int j = 0;

    // return arrays of connection equal to the data needed
    struct Operation* tmp = alloc_array(sizeof(struct Operation)*conn->db->max_rows, conn->db->max_rows);
    if(!tmp)
	exit(1);
    switch(dataType){
    case MONEY:
	for(i = 0; i < conn->db->max_rows; ++i) {
	    if(conn->db->rows[i].money == data.i){
		memcpy(&tmp[j], &conn->db->rows[i], sizeof(struct Operation));
		j++;
	    }
	}
	break;
    case TIME:
	for(i = 0; i < conn->db->max_rows; ++i) {
	    if(conn->db->rows[i].time == data.t){
		memcpy(&tmp[j], &conn->db->rows[i], sizeof(struct Operation));
		j++;
	    }
	}
	break;
    case OPERATION_TYPE:
	for(i = 0; i < conn->db->max_rows; ++i) {
	    if(conn->db->rows[i].operation_type){
		if(strncmp(conn->db->rows[i].operation_type, data.ucptr, conn->db->max_data) == 0) {
		    memcpy(&tmp[j], &conn->db->rows[i], sizeof(struct Operation));
		    tmp[j].operation_type = malloc(sizeof(char) * conn->db->max_data);
		    strncpy(tmp[j].operation_type, data.ucptr, conn->db->max_data);
		    ++j;
		}
	    }
	}
	break;
    default :
	printf("NO OPERATION CORRESPONDING TO THE ONE SPECIFIED!");
	break;
    }
    return (struct Operation*) realloc_array(tmp, sizeof(struct Operation)*j, j);
    
}

/*
 * reset database to default value
 * return sucess or fail.
 */
void reset_database(struct Connection* conn) {
    //recursively set database to 0.
    int i = 0;
    struct Operation operation = {.ID = 0, .is_set = 0, .operation_type = NULL, .money = 0, .time = 0 };
    
    for(i = 0; i < conn->db->max_rows ; ++i) {
	memcpy(&conn->db->rows[i], &operation, sizeof(struct Operation));
    }
    
}

/*
 * Delete database file but not memory
 * return sucess or fail.
 */
void delete_database(struct Connection* conn){

    char filedescriptor[512] = {"/proc/self/fd/"};

    char path_ptr[512] = {'\0'};

    snprintf(path_ptr, sizeof(filedescriptor), "%d", fileno(conn->file)); //revoir attribution de la mémoire.
    strcat(filedescriptor, path_ptr);
    memset(path_ptr, 0, sizeof(char)*20);
    int ret = readlink(filedescriptor, path_ptr, 512);
    if(ret < 0)
	exit(1);


    if(!ret)
	exit(1);

    if(access(path_ptr, F_OK) == 0) {
	ret = remove(path_ptr);
	printf("File deleted : %s", path_ptr);
	if(ret)
	    exit(1);
    }

    close_database(conn);
}
