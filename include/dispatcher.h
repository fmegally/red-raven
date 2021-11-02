#ifndef DISPATCHER_H_
#define DISPATCHER_H_

extern handler_func_t handler_table[ID_TABLE_SIZE];
int8_t dispatch(struct telegram *tg, handler_func_t table[]);

#endif
