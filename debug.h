#ifndef _DEBUG_H
#define _DEBUG_H

#include "database.h"

void debug_database(int* routine);
void routine_check_memory_money(void); 
void routine_check_memory_operation(void);
void routine_read_file(void); 
void routine_write_file_human_readable(void); 
void routine_test_write_read(void); 
void debug_read_database(struct Connection* connRead, struct Connection* connWritten); 
void debug_write_database(struct Connection* conn); 
#endif
