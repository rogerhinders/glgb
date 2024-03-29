#ifndef TABLES_H
#define TABLES_H
static const u8 op_t[] = {   4,12, 8, 8, 4, 4, 8, 4,20, 8, 8, 8, 4, 4, 8, 4, 4,
                            12, 8, 8, 4, 4, 8, 4, 8, 8, 8, 8, 4, 4, 8, 4, 8,12,
                             8, 8, 4, 4, 8, 4, 8, 8, 8, 8, 4, 4, 8, 4, 8,12, 8,
                             8,12,12,12, 4, 8, 8, 8, 8, 4, 4, 8, 4, 4, 4, 4, 4,
                             4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4,
                             4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4,
                             8, 4, 4, 4, 4, 4, 4, 4, 8, 4, 8, 8, 8, 8, 8, 8, 4,
                             8, 4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
                             4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4, 4,
                             4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4, 4, 4,
                             4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4,
                             4, 4, 4, 8, 4, 8,12,12,12,12,16, 8,32, 8, 8,12, 0,
                            12,12, 8,32, 8,12,12, 0,12,16, 8,32, 8, 8,12, 0,12,
                             0, 8,32,12,12, 8, 0, 0,16, 8,32,16, 4,16, 0, 0, 0,
                             8,32,12,12, 8, 4, 0,16, 8,32,12, 8,16, 4, 0, 0, 8,
                            32 };

static const u8 op_t_cb[] = {    8, 8, 8, 8, 8, 8,16, 8, 8, 8, 8, 8, 8, 8,16, 8, 8,
                                 8, 8, 8, 8, 8,16, 8, 8, 8, 8, 8, 8, 8,16, 8, 8, 8,
                                 8, 8, 8, 8,16, 8, 8, 8, 8, 8, 8, 8,16, 8, 8, 8, 8,
                                 8, 8, 8,16, 8, 8, 8, 8, 8, 8, 8,16, 8, 8, 8, 8, 8,
                                 8, 8,16, 8, 8, 8, 8, 8, 8, 8,16, 8, 8, 8, 8, 8, 8,
                                 8,16, 8, 8, 8, 8, 8, 8, 8,16, 8, 8, 8, 8, 8, 8, 8,
                                16, 8, 8, 8, 8, 8, 8, 8,16, 8, 8, 8, 8, 8, 8, 8,16,
                                 8, 8, 8, 8, 8, 8, 8,16, 8, 8, 8, 8, 8, 8, 8,16, 8,
                                 8, 8, 8, 8, 8, 8,16, 8, 8, 8, 8, 8, 8, 8,16, 8, 8,
                                 8, 8, 8, 8, 8,16, 8, 8, 8, 8, 8, 8, 8,16, 8, 8, 8,
                                 8, 8, 8, 8,16, 8, 8, 8, 8, 8, 8, 8,16, 8, 8, 8, 8,
                                 8, 8, 8,16, 8, 8, 8, 8, 8, 8, 8,16, 8, 8, 8, 8, 8,
                                 8, 8,16, 8, 8, 8, 8, 8, 8, 8,16, 8, 8, 8, 8, 8, 8,
                                 8,16, 8, 8, 8, 8, 8, 8, 8,16, 8, 8, 8, 8, 8, 8, 8,
                                16, 8, 8, 8, 8, 8, 8, 8,16, 8, 8, 8, 8, 8, 8, 8,16,
                                 8 };
static const u8 op_l[] = { 01,03,01,01,01,01,02,01,03,01,01,01,01,01,02,01,01,
                           03,01,01,01,01,02,01,02,01,01,01,01,01,02,01,02,03,
                           01,01,01,01,02,01,02,01,01,01,01,01,02,01,02,02,01,
                           01,01,01,02,01,02,01,01,01,01,01,02,01,01,01,01,01,
                           01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,
                           01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,
                           01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,
                           01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,
                           01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,
                           01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,
                           01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,01,
                           01,01,01,01,01,01,01,03,03,03,01,02,01,01,01,03,02,
                           03,03,02,01,01,01,03,01,03,01,02,01,01,01,03,01,03,
                           01,02,01,02,01,01,01,01,01,02,01,02,01,03,01,01,01,
                           02,01,02,01,01,01,01,01,02,01,02,01,03,01,01,01,02,
                           01 };
#endif // TABLES_H