#include <stdio.h>
#include <stdbool.h>
#include <unistd.h> // ����usleep����

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_EXIT,
    STATE_COUNT
} StateID;

typedef struct State State;

struct State {
    StateID id;
    void (*enter)(void);
    void (*execute)(void);
    void (*exit)(void);
};

State *current_state;

void enterStateA(void);
void executeStateA(void);
void exitStateA(void);
void enterStateB(void);
void executeStateB(void);
void exitStateB(void);
void enterStateC(void);
void executeStateC(void);
void exitStateC(void);
void enterStateExit(void);
void executeStateExit(void);
void exitStateExit(void);

State states[STATE_COUNT] = {
    {STATE_A, enterStateA, executeStateA, exitStateA},
    {STATE_B, enterStateB, executeStateB, exitStateB},
    {STATE_C, enterStateC, executeStateC, exitStateC},
    {STATE_EXIT, enterStateExit, executeStateExit, exitStateExit}
};

void changeState(StateID new_state) {
    if (current_state != NULL) {
        current_state->exit();
    }
    current_state = &states[new_state];
    current_state->enter();
}

void enterStateA(void) { printf("Entering State A\n"); }
void executeStateA(void) {
    printf("Executing State A\n");
    // ʾ��������ĳ�������ж��Ƿ�Ҫ�л���״̬B
    if (/* ���� */ false) {
        changeState(STATE_B);
    }
}
void exitStateA(void) { printf("Exiting State A\n"); }
void enterStateB(void) { printf("Entering State B\n"); }
void executeStateB(void) { printf("Executing State B\n"); }
void exitStateB(void) { printf("Exiting State B\n"); }
void enterStateC(void) { printf("Entering State C\n"); }
void executeStateC(void) { printf("Executing State C\n"); }
void exitStateC(void) { printf("Exiting State C\n"); }
void enterStateExit(void) { printf("Entering Exit State\n"); }
void executeStateExit(void) { printf("Executing Exit State\n"); }
void exitStateExit(void) { printf("Exiting Exit State\n"); }

int main() {
    bool running = true;
    changeState(STATE_A);

    while (running) {
        current_state->execute();
        
        // ����Ƿ�Ϊ�˳�״̬
        if (current_state->id == STATE_EXIT) {
            running = false;
        }
        
        usleep(10000); // �ȴ�10����
    }

    return 0;
}
