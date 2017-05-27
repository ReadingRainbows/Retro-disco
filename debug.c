#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "debug.h"
#include "database.h"
#include "fat_pointers.h"

static struct Connection* conn = NULL;

struct Connection* init_debug(void) {
    struct Connection* connection = NULL; 
    connection = open_database("bob", 512, 512);
    return connection;
}

void debug_database(int* routine) {
    struct Operation* get_conn_to_free = NULL;
    Data_type_union case0 = {.i = 100};
    Data_type_union case1 = {.ucptr = "substract"};
    Data_type_union case2 = {.i = 5};
    Data_type_union case2_1 = {.ucptr = "add"};
    struct Connection* connection = NULL;
    int i = 0;
    int j = 0;
    

    if(!conn) {
	conn = init_debug();
    }

    switch(*routine) {
    case 0:
	set_database(conn, 0, 100, "add");
	set_database(conn, 248, 100, "add");
	get_conn_to_free = (struct Operation*) get_database(conn, case0, MONEY);
	printf("\nMontant 1: %d \nMontant 2: %d", get_conn_to_free[0].money, get_conn_to_free[1].money);
	write_database(conn);
	for(j = 0; j < get_array_length(get_conn_to_free); ++j) {
	    free(get_conn_to_free[j].operation_type);
	}
	dealloc_array(get_conn_to_free);
	break;
    case 1:
	set_database(conn, 1, 99, "substract");
	set_database(conn, 365, 98, "substract");
	get_conn_to_free = (struct Operation*) get_database(conn, case1, OPERATION_TYPE);
	printf("\nOperation 1: %s \nOperation 2: %s ", get_conn_to_free[0].operation_type, get_conn_to_free[1].operation_type);
	write_database(conn);
	for(j = 0; j < get_array_length(get_conn_to_free); ++j) {
	    free(get_conn_to_free[j].operation_type);
	}
	dealloc_array(get_conn_to_free);
	break;
    case 2:
        connection = open_database("robert", 512, 512);
	set_database(connection, 2, 5, "set");
	set_database(connection, 250, 10000, "add");
	write_database(connection);
	close_database(connection);
	connection = open_database("robert", 512, 512);
	read_database(connection);
        get_conn_to_free = get_database(connection, case2, MONEY);
	printf("\nMontant 1: %d ", get_conn_to_free[0].money);
	for(j = 0; j < get_array_length(get_conn_to_free); ++j) {
	    free(get_conn_to_free[j].operation_type);
	}
	dealloc_array(get_conn_to_free);
	get_conn_to_free = get_database(connection, case2_1, OPERATION_TYPE);
	printf("\nOperation 1: %s", get_conn_to_free[0].operation_type);
	for(j = 0; j < get_array_length(get_conn_to_free); ++j) {
	    free(get_conn_to_free[j].operation_type);
	}
	dealloc_array(get_conn_to_free);
	delete_database(connection);
	break;
    case 3:
	test_write_read();
	break;
    case 4:
	connection = open_database("richard",512,512);
	for(i = 0; i < connection->db->max_data; ++i) {
	    set_database(connection, i, i, "set");
	}
	debug_write_database(connection);
	delete_database(connection);
	break;
    default:
	close_database(conn);
	break;
    }

}

void test_write_read(void) {
    struct Connection* connectionWritten = NULL;
    struct Connection* connectionRead = NULL; 

    connectionWritten = open_database("george",512,512);
    connectionRead = open_database("gontrand",512,512);
    set_database(connectionWritten, 270, 88, "substract");
    set_database(connectionWritten, 288, 70, "add");
    write_database(connectionWritten);
    debug_read_database(connectionRead,connectionWritten);
    printf("end");
    delete_database(connectionWritten);
    delete_database(connectionRead);
}

void debug_read_database(struct Connection* connRead, struct Connection* connWritten) {
    int rc = 0;
    int i = 0;
    rc = fread(&connRead->db->max_data, sizeof(int), 1, connWritten->file);
    rc = fread(&connRead->db->max_rows, sizeof(int), 1, connWritten->file);

    for(i = 0; i < conn->db->max_rows; ++i) {
	rc = 0;
	rc += fread(&connRead->db->rows[i].ID, sizeof(unsigned int), 1, connWritten->file);
	rc += fread(&connRead->db->rows[i].is_set, sizeof(unsigned int), 1, connWritten->file);
	rc += fread(&connRead->db->rows[i].operation_type, sizeof(char) * conn->db->max_data, 1, connWritten->file);
	rc += fread(&connRead->db->rows[i].money, sizeof(unsigned int), 1, connWritten->file);
	rc += fread(&connRead->db->rows[i].time, sizeof(time_t), 1, connWritten->file);
    } 
}

void debug_write_database(struct Connection* conn) {
    int wc = 0;
    int i = 0;
    int lenght = 0;
    int ret = 0;
    char dataConversion[100] = {0};

    rewind(conn->file); //return file position to 0.

    //write column and rows in file to be sure to read it correctly after
    snprintf(dataConversion, sizeof(char) * 100, "%d", (int) conn->db->max_data); 
    lenght = strlen(dataConversion);
    dataConversion[lenght] = '\n';
    dataConversion[lenght+1] = '\0';
    wc += fwrite(dataConversion, sizeof(char)* lenght+1, 1, conn->file);

    snprintf(dataConversion, sizeof(char)*100, "%d", (int) conn->db->max_rows); 
    lenght = strlen(dataConversion);
    dataConversion[lenght] = '\n';
    dataConversion[lenght+1] = '\0';
    wc += fwrite(dataConversion, sizeof(char)*lenght+1, 1, conn->file);

    //right every single case "Operation" in the database
    for(i = 0; i < conn->db->max_rows; ++i) {
	snprintf(dataConversion, sizeof(char)*100, "%d", conn->db->rows[i].ID);
	lenght = strlen(dataConversion);
	dataConversion[lenght] = '\n';
	dataConversion[lenght+1] = '\0';
	wc += fwrite(dataConversion, sizeof(char)*lenght+1, 1, conn->file);

	snprintf(dataConversion, sizeof(char)*100, "%d", conn->db->rows[i].is_set);
	lenght = strlen(dataConversion);
	dataConversion[lenght] = '\n';
	dataConversion[lenght+1] = '\0';
	wc += fwrite(dataConversion, sizeof(char)*lenght+1, 1, conn->file);

	strncpy(dataConversion, conn->db->rows[i].operation_type, sizeof(char)*100);
	lenght = strlen(dataConversion);
	dataConversion[lenght] = '\n';
	dataConversion[lenght+1] = '\0';
	wc += fwrite(dataConversion, sizeof(char)*lenght+1, 1, conn->file);

	snprintf(dataConversion, sizeof(char)*100, "%f", (float)conn->db->rows[i].time);
	lenght = strlen(dataConversion);
	dataConversion[lenght] = '\n';
	dataConversion[lenght+1] = '\0';
	wc += fwrite(dataConversion, sizeof(char)*lenght+1, 1, conn->file);

    }

    //error handling for wc!

    ret = fflush(conn->file);
    if(ret != 0)
	exit(1);

}
