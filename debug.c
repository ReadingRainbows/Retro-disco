#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "debug.h"
#include "database.h"
#include "fat_pointers.h"

static struct Connection* conn = NULL;

void debug_database(int* routine) {
    
    switch(*routine) {
    case 0:
	routine_check_memory_money();
	break;
    case 1:
	routine_check_memory_operation();
	break;
    case 2:
	routine_read_file();
	break;
    case 3:
	routine_test_write_read();
	break;
    case 4:
	routine_write_file_human_readable();
	break;
    default:
	if(conn) {
	    close_database(conn);
	}
	break;
    }

}

void routine_check_memory_money(void) {
    struct Operation* get_conn_to_free = NULL;
    struct Connection* conn = NULL;
    Data_type_union case0 = {.i = 100};
    conn = open_database("bob", 512, 512);
    set_database(conn, 0, 100, "add");
    set_database(conn, 248, 100, "add");
    get_conn_to_free = (struct Operation*) get_database(conn, case0, MONEY);
    printf("\nMontant 1: %d \nMontant 2: %d", get_conn_to_free[0].money, get_conn_to_free[1].money);
    write_database(conn);
    dealloc_array(get_conn_to_free);
    close_database(conn);
}

void routine_check_memory_operation(void) {
    struct Operation* get_conn_to_free = NULL;
    struct Connection* conn = NULL;
    Data_type_union case1 = {.ucptr = "substract"};
    int j = 0;
    conn = open_database("bob", 512, 512);
    set_database(conn, 1, 99, "substract");
    set_database(conn, 365, 98, "substract");
    get_conn_to_free = (struct Operation*) get_database(conn, case1, OPERATION_TYPE);
    printf("\nOperation 1: %s \nOperation 2: %s ", get_conn_to_free[0].operation_type, get_conn_to_free[1].operation_type);
    write_database(conn);
    for(j = 0; j < get_array_length(get_conn_to_free); ++j) {
	free(get_conn_to_free[j].operation_type);
    }
    dealloc_array(get_conn_to_free);
    close_database(conn);
}

void routine_read_file(void) {
    struct Operation* get_conn_to_free = NULL;
    Data_type_union case2 = {.i = 5};
    Data_type_union case2_1 = {.ucptr = "add"};
    struct Connection* connection = NULL;
    int j = 0;
    connection = open_database("robert", 512, 512);
    set_database(connection, 2, 5, "set");
    set_database(connection, 250, 10000, "add");
    write_database(connection);
    close_database(connection);
    connection = open_database("robert", 512, 512);
    read_database(connection);
    get_conn_to_free = get_database(connection, case2, MONEY);
    printf("\nMontant 1: %d ", get_conn_to_free[0].money);
    dealloc_array(get_conn_to_free);
    get_conn_to_free = get_database(connection, case2_1, OPERATION_TYPE);
    printf("\nOperation 1: %s", get_conn_to_free[0].operation_type);
    for(j = 0; j < get_array_length(get_conn_to_free); ++j) {
	free(get_conn_to_free[j].operation_type);
    }
    dealloc_array(get_conn_to_free);
    delete_database(connection);
}

void routine_write_file_human_readable(void) {
    struct Connection* connection = NULL;
    int i = 0;
    connection = open_database("richard",512,512);
    for(i = 0; i < connection->db->max_data; ++i) {
	set_database(connection, i, i, "set");
    }
    debug_write_database(connection);
    delete_database(connection);
}

void routine_test_write_read(void) {
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

    for(i = 0; i < connRead->db->max_rows; ++i) {
	rc = 0;
	rc += fread(&connRead->db->rows[i].ID, sizeof(unsigned int), 1, connWritten->file);
	rc += fread(&connRead->db->rows[i].is_set, sizeof(unsigned int), 1, connWritten->file);
	rc += fread(&connRead->db->rows[i].operation_type, sizeof(char) * connRead->db->max_data, 1, connWritten->file);
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
