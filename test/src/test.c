#include <stdio.h>
#include "../../emulator/emulator.h"
#include "../../assembler/assembler.h"

int main(void) {
    assemble("fibb.asm", "fibb.obj", "fibb.dmp");
    load("fibb.obj");
    run(1);
}
