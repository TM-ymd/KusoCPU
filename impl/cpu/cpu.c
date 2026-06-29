#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct cpu_register {
    char *name;
    int value;
};

struct cpu_register reg[10] = {
    {"R0", 0x00}, // program counter
    {"R1", 0x00}, // instruction register
    {"R2", 0x00}, // flag register
    {"R3", 0x00}, // stack register
    {"R4", 0x00}, // general purpose register 1
    {"R5", 0x00}, // accumulator
    {"R6", 0x00}, // base register
    {"R7", 0x00}, // index register
    {"R8", 0x00}, // general purpose register 2
    {"R9", 0x00}, // general purpose register 3
};


void cpu_exec_add(char val1, char val2, char register_index) {
    // char は文字でなく 1 バイトの数字として扱うので操作不要
    reg[register_index].value = val1 + val2;
}

void cpu_exec_sub(char val1, char val2, char register_index) {
    // char は文字でなく 1 バイトの数字として扱うので操作不要
    reg[register_index].value = val1 - val2;
}

/***
まずは即値だけ
 ***/
void cpu_exec_mov(char val, char register_index) {
    reg[register_index].value = val;
}

void cpu_exec_jmp(char label) {
    (void)label;
    printf("TODO: 命令リストでのテスト時に実装\n");
}

void cpu_exec_cmp(char register1_index, char register2_index) {
    reg[2].value = reg[register1_index].value - reg[register2_index].value;
}

int cpu_instruction_execute(char *instruction) {
    // ADD
    if (instruction[0] == 0x01) {
        cpu_exec_add(instruction[1], instruction[2], instruction[3]);
    // SUB
    } else if (instruction[0] == 0x02) {
        cpu_exec_sub(instruction[1], instruction[2], instruction[3]);
    // MOV
    } else if (instruction[0] == 0x03) {
        cpu_exec_mov(instruction[1], instruction[2]);
    // JMP
    } else if (instruction[0] == 0x04) {
        cpu_exec_jmp(instruction[1]);
    // CMP
    } else if (instruction[0] == 0x05) {
        cpu_exec_cmp(instruction[1], instruction[2]);
    } else {
        printf("Invalid instruction\n");
    }
    return 0;
}

char *cpu_instruction_fetch_test(int instruction_number) {
    char *instruction = malloc(sizeof(char) * 5);
    switch (instruction_number) {
        case 0x01: // ADD
            instruction[1] = 10;
            instruction[2] = 20;
            instruction[3] = 0x04;
            break;
        case 0x02: // SUB
            instruction[1] = 10;
            instruction[2] = 20;
            instruction[3] = 0x08;
            break;
        case 0x03: // MOV
            instruction[1] = 10;
            instruction[2] = 0x09;
            break;
        case 0x04: // JMP
            // TODO: 命令リストでのテスト時に具体的なジャンプ先を指定
            instruction[1] = 0x00;
            break;
        case 0x05: // CMP
            instruction[1] = 0x00;
            instruction[2] = 0x04;
            break;
        default:
            printf("Invalid instruction number\n");
            break;
    }
    instruction[0] = instruction_number;
    return instruction;
}

char *cpu_instruction_fetch(int instruction_number) {
    return cpu_instruction_fetch_test(instruction_number);
    // TODO: 実際のバイト列を 1 バイトずつ読み込んで instruction に格納する処理の実装
}

void show_all_registers(void) {
    for (int i = 0; i < 10; i++) {
        printf("%s: %02X\n", reg[i].name, reg[i].value);
    }
}

void show_instruction(char *instruction) {
    for (int i = 0; i < 5; i++) {
        printf("%02X\n", instruction[i]);
    }
}

int check_register_value(void) {
    if (reg[4].value != 30) {
        printf("R4: %02X(expected: 30)\n", reg[4].value);
        return 0;
    }
    if (reg[8].value != -10) {
        printf("R8: %02X(expected: -10)\n", reg[8].value);
        return 0;
    }
    if (reg[9].value != 10) {
        printf("R9: %02X(expected: 10)\n", reg[9].value);
        return 0;
    }
    printf("pass: JMP\n");
    if (reg[2].value != -30) {
        printf("failed: CMP (R0 - R4 = -30, but %02X)\n", reg[2].value);
        return 0;
    }
    return 1;
}

// わざとレジスタ2に１を代入してテスト失敗させる用の関数
void set_register_2_to_1(void) {
    reg[2].value = 1;
}

int test_cpu(void) {
    int return_value = 0;

    show_all_registers();
    // these functions below use malloc, so we need to free it
    char *instructions[5];
    struct cpu_register *pc = &reg[0];
    for (; pc->value < 5; pc->value++) {
        instructions[pc->value] = cpu_instruction_fetch(pc->value);
        show_instruction(instructions[pc->value]);
        cpu_instruction_execute(instructions[pc->value]);
    }
    show_all_registers();
    //set_register_2_to_1();
    if (!check_register_value()) {
        printf("failed\n");
        return_value = 1;
        goto free_and_return;
    }
    printf("success\n");
free_and_return:
    for (int i = 0; i < 5; i++) {
        free(instructions[i]);
    }
    return return_value;
}

int main(void) {
    return test_cpu();
}
