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

enum cpu_instruction_type {
    CPU_INSTRUCTION_TYPE_ADD = 0,
    CPU_INSTRUCTION_TYPE_SUB = 1,
    CPU_INSTRUCTION_TYPE_MOV = 2,
    CPU_INSTRUCTION_TYPE_JMP = 3,
    CPU_INSTRUCTION_TYPE_CMP = 4,
};

void cpu_exec_add(char *val1, char *val2, char *register_name) {
    int val1_int = strtol(val1, NULL, 10);
    int val2_int = strtol(val2, NULL, 10);
    int register_index = atoi(register_name + 1);
    reg[register_index].value = val1_int + val2_int;
}

void cpu_exec_sub(char *val1, char *val2, char *register_name) {
    int val1_int = strtol(val1, NULL, 10);
    int val2_int = strtol(val2, NULL, 10);
    int register_index = atoi(register_name + 1);
    reg[register_index].value = val1_int - val2_int;
}

/***
まずは即値だけ
 ***/
void cpu_exec_mov(char *val, char *register_name) {
    int val_int = strtol(val, NULL, 10);
    int register_index = atoi(register_name + 1);
    reg[register_index].value = val_int;
}

void cpu_exec_jmp(char *label) {
    (void)label;
    printf("TODO: 命令リストでのテスト時に実装\n");
}

void cpu_exec_cmp(char *register1, char *register2) {
    int register1_index = atoi(register1 + 1);
    int register2_index = atoi(register2 + 1);
    reg[2].value = reg[register1_index].value - reg[register2_index].value;
}

int cpu_instruction_parser(char **instruction) {
    if (strcmp(instruction[0], "ADD") == 0) {
        cpu_exec_add(instruction[1], instruction[2], instruction[3]);
    } else if (strcmp(instruction[0], "SUB") == 0) {
        cpu_exec_sub(instruction[1], instruction[2], instruction[3]);
    } else if (strcmp(instruction[0], "MOV") == 0) {
        cpu_exec_mov(instruction[1], instruction[2]);
    } else if (strcmp(instruction[0], "JMP") == 0) {
        cpu_exec_jmp(instruction[1]);
    } else if (strcmp(instruction[0], "CMP") == 0) {
        cpu_exec_cmp(instruction[1], instruction[2]);
    } else {
        printf("Invalid instruction\n");
    }
    return 0;
}

char **cpu_instruction_fetch_test(int instruction_number) {
    char **instruction = malloc(sizeof(char *) * 5);
    switch (instruction_number) {
        case CPU_INSTRUCTION_TYPE_ADD:
            instruction[0] = "ADD";
            instruction[1] = "10";
            instruction[2] = "20";
            instruction[3] = "R4";
            instruction[4] = NULL;
            break;
        case CPU_INSTRUCTION_TYPE_SUB:
            instruction[0] = "SUB";
            instruction[1] = "10";
            instruction[2] = "20";
            instruction[3] = "R8";
            instruction[4] = NULL;
            break;
        case CPU_INSTRUCTION_TYPE_MOV:
            instruction[0] = "MOV";
            instruction[1] = "10";
            instruction[2] = "R9";
            instruction[3] = NULL;
            break;
        case CPU_INSTRUCTION_TYPE_JMP:
            instruction[0] = "JMP";
            instruction[1] = "label";
            instruction[2] = NULL;
            break;
        case CPU_INSTRUCTION_TYPE_CMP:
            instruction[0] = "CMP";
            instruction[1] = "R0";
            instruction[2] = "R4";
            instruction[3] = NULL;
            break;
        default:
            printf("Invalid instruction number\n");
            break;
    }
    return instruction;
}

char **cpu_instruction_fetch(int instruction_number) {
    return cpu_instruction_fetch_test(instruction_number);
}

void show_all_registers(void) {
    for (int i = 0; i < 10; i++) {
        printf("%s: %02X\n", reg[i].name, reg[i].value);
    }
}

void show_instruction(char **instruction) {
    for (int i = 0; instruction[i] != NULL; i++) {
        printf("%s\n", instruction[i]);
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
    char **instructions[5];
    for (int i = 0; i < 5; i++) {
        instructions[i] = cpu_instruction_fetch(i);
        show_instruction(instructions[i]);
        cpu_instruction_parser(instructions[i]);
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
