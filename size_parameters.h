#ifndef SIZEPARAMETERS_H
#define SIZEPARAMETERS_H

extern int N_keywords;
extern int N_max_ids;
extern int N_row_ids;
extern int BF_length;

#define N_HASH 1                            //Equal to N_Threads     (Also change in BloomFilter.h)
#define MAX_BF_BIN_SIZE 64                  //6 bits

#endif // SIZEPARAMETERS_H
