#ifndef _DEBUG_H
#define _DEBUG_H

void debug_database(int* routine);
struct Connection* init_debug(void);
void test_write_read(void); 
void debug_read_database(struct Connection* connRead, struct Connection* connWritten); 
void debug_write_database(struct Connection* conn); 
#endif
