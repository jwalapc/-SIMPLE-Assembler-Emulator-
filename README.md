# SIMPLE-Assembler-Emulator
An assembler and emulator based on SIMPLE instruction set.

Assembler is a 2-pass assembler, which uses data structures in structures.h header file to store labels and data.
* First pass: Identifies labels and data used in the .asm file, and prepare tables for them. Also checks for syntax errors.
* Second pass: Checks for syntax errors and if no errors, generate object code and listing file

Emulator is a program that executes the generated machine code, using mnemonics declared in mnemonics.h header as functions.
* Trace: Prints the state of registers after execution of each line.
* Before: Prints the memory dump before execution of the program.
* After: Prints the memory dump after execution of the program.

This assembly language is for a machine with four registers,
* Two registers, A & B, arranged as an internal stack.
* A program counter, PC
* A stack pointer, SP

| Mnemonic | Opcode | Operand | Formal Specs | Description |
|----------|--------|---------|--------------|-------------|
| data | | value | |Reserve a memory location, initialized to the value specified value|
|ldc| 0| value| B := A; A := value;| Load accumulator with the value specified|
|adc| 1 |value| A := A + value;| Add the value specified to the accumulator|
|ldl| 2 |offset| B := A; A := memory[SP + offset];| Load local|
|stl| 3 |offset| memory[SP + offset] := A; A := B;| Store local|
|ldnl| 4 |offset| A := memory[A + offset];| Load non-local|
|stnl| 5 |offset| memory[A + offset] := B;| Store non-local|
|add| 6 | |A := B + A;| Addition|
|sub |7 | |A := B - A;| Subtraction|
|shl |8 | |A := B << A;| Shift left|
|shr |9 | |A := B >> A;| Shift right|
|adj| 10 |value|SP := SP + value;| Adjust SP|
|a2sp |11 | |SP := A; A := B; |Transfer A to SP|
|sp2a |12 | |B := A; A := SP; |Transfer SP to A|
|call |13 |offset| B := A; A := PC; PC := PC + offset;|Call procedure|
|return| 14| |PC := A; A := B;| Return from procedure
|brz| 15| offset| if A == 0 then PC := PC + offset;| If accumulator is zero, branch to specified offset|
|brlz| 16| offset| if A < 0 then PC := PC + offset;| If accumulator is less than zero, branch to specified offset|
|br |17 |offset |PC := PC + offset; |Branch to specified offset|
|HALT|18 | | |Stop the emulator. This is not a 'real' instruction, but needed to tell your emulator when to finish.|


My Implementation has separated Code Segment and Data segment, memory is allocated according to the data variables used + OFFSET, using dynamic memory allocation.
