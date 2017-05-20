#include <stdlib.h>
#include "budget.h"
#include "database.h"
#include "fat_pointers.h"

static struct Connection* conn = NULL;

struct Connection* init_debug(void) {
    struct Connection* connection = malloc(sizeof(struct Connection));
    connection = open_database("bob");
    populate_database(connection);
    return connection;
}

void debug_database(int* routine) {
    struct Operation* get_conn_to_free = NULL;
    Data_type_union case0 = {.i = 100};
    Data_type_union case1 = {.ucptr = "substract"};
    Data_type_union case2 = {.i = 5};
    Data_type_union case2_1 = {.ucptr = "add"};
    struct Connection* connection = malloc(sizeof(struct Connection));
    

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
	dealloc_array(get_conn_to_free);
	break;
    case 1:
	set_database(conn, 1, 99, "substract");
	set_database(conn, 365, 98, "substract");
	get_conn_to_free = (struct Operation*) get_database(conn, case1, OPERATION_TYPE);
	printf("\nOperation 1: %s \nOperation 2: %s ", get_conn_to_free[0].operation_type, get_conn_to_free[1].operation_type);
	write_database(conn);
	dealloc_array(get_conn_to_free);
	break;
    case 2:
        connection = open_database("robert");
	set_database(connection, 2, 5, "set");
	set_database(connection, 300, 10000, "add");
	write_database(connection);
	close_database(connection);
	connection = open_database("robert");
	read_database(connection);
        get_conn_to_free = get_database(connection, case2, MONEY);
	printf("\nMontant 1: %d ", get_conn_to_free[0].money);
	dealloc_array(get_conn_to_free);
	get_conn_to_free = get_database(connection, case2_1, OPERATION_TYPE);
	printf("\nOperation 1: %s", get_conn_to_free[0].operation_type);
	dealloc_array(get_conn_to_free);
	delete_database(connection);
	break;
    default:
	close_database(conn);
	break;
    }

}
