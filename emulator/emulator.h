#ifndef _EMULATOR_H_
#define _EMULATOR_H_

#include <stdint.h>
struct CPU {
    int16_t r[8];
    uint16_t pc;
    uint8_t cc;
};
void disassemble(int16_t ins, char* out);
void execute(int16_t ins, char* out);
struct CPU get_cpu(void);
int16_t get_mem(int add);
void print_cpu(void);
void load(char* fn);
void step(void);
void run(_Bool dbg);
#endif
