#ifndef _LECTRED_H
#define _LECTRED_H
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
 

struct lectred;

struct lectred* lectred_init();

void begin_read(struct lectred*);
void end_read(struct lectred*);
void begin_write(struct lectred*);
void end_write(struct lectred*);

void lectred_destroy(struct lectred*);

#endif

