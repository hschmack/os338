/*
 * Hayden Schmackpfeffer - hbs25
 * Homework 4
 * 10/28/15
 * Uses methods from the Spring 2015 HW 4 solution
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// This serves as both the key for the semaphores and the shared memory
// (the same key fits two different locks)
#define SEMAPHORE_KEY        0xFA2B

// The position of the various semaphores that we are using in the 
// "semaphore array" that semget gets
#define SEMAPHORE_MUTEX      0
#define SEMAPHORE_WLIST      1
#define NUMBER_OF_SEMAPHORES 2

// To distinguish withdrawer process from east bound when forking
#define WITHDRAW             1
#define DEPOSIT              2

// Amount of iterations of a loop that wastes time
#define WASTED_TIME          7000

// Function headers

//Function to make a new Bank customer process (Depositer or Withdrawer)
void customer_fork(int withdraw_or_deposit);

// //What a Depositer executes
void depositing_cust();

// //What a Withdrawer executes
void withdrawing_cust();

// Semaphore functions
void semaphore_wait(int semid, int semnumber);
void semaphore_signal(int semid, int semnumber);
int create_semaphore(int value);

// A staller function that runs an empty for loop iterations times
void stall(int iterations);

int get_semid(key_t semkey);
int get_shmid(key_t shmkey);
int firstRequestAmt(int list[]);
void addEndOfList(int list[], int addition, int waitCnt);
void deleteFirstRequest(int list[], int length);

// This union is required by semctl(2)
// http://linux.die.net/man/2/semctl
union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                (Linux-specific) */
};


// Shared memory struct to store 
struct shared_variable_struct {
    int wcount;
    int balance;
    int waitingCustomers[100];
};

// Main
int main(int argc, char *argv[]) {
    printf("*** Hello World! I am process %d.\n", getpid());

    //TODO check if arguments are correct

    //to be used in semaphore creation
    union semun semaphore_values;

    //initial values
    unsigned short semaphore_init_values[NUMBER_OF_SEMAPHORES];
    semaphore_init_values[SEMAPHORE_MUTEX] = 1;
    semaphore_init_values[SEMAPHORE_WLIST] = 0;
    semaphore_values.array = semaphore_init_values;

    //turn the initial values array into initialized semaphores
    int semid = get_semid((key_t)SEMAPHORE_KEY);
    if (semctl(semid, SEMAPHORE_MUTEX, SETALL, semaphore_values) == -1) {
        perror("semctl failed");
        exit(EXIT_FAILURE);
    }

    // Shared Memory
    int shmid = get_shmid((key_t)SEMAPHORE_KEY);
    struct shared_variable_struct * shared_variables = shmat(shmid, 0, 0);

    //Set the initial values of the shared memory
    shared_variables->wcount = 0;
    shared_variables->balance = 0;
    //waitingCustomers should initialize to 100 elements of 0

}


void customer_fork(int withdraw_or_deposit) {
    pid_t child_pid;
    child_pid = fork();
    if (child_pid == -1){
        perror("Fork Failed");
        exit(EXIT_FAILURE);
    } else if (child_pid == 0) {
        //here if child
        if(withdraw_or_deposit == WITHDRAW) {
            withdrawing_cust();
        } else if (withdraw_or_deposit == DEPOSIT) {
            depositing_cust();
        } else {
            printf("INVALID OPERATION: Why are you even at the bank?\n");
            exit(EXIT_FAILURE);
        }
    } else {
        //parent do nothing
        return;
    }
}

// Just stall with an empty for loop
void stall(int iterations){
    int i;
    for(i = 0; i < iterations; i++){
        ;
    }
}

void withdrawing_cust(){

}

void depositing_cust(){
    
}

// These two functions are wrapper functions for the System-V
// style semaphores that were talked about in class. 
// They implement semaphore wait and signal functions as discussed in class.
// Used in 2013 Assignment 5 solutions.
void semaphore_wait(int semid, int semnumber) {
    // declare a sembuf
    struct sembuf wait_buffer;
    // We will perfom an operation on the semnumber semaphore of semid
    wait_buffer.sem_num = semnumber;
    //We will subtract 1 from the semaphore
    wait_buffer.sem_op = -1;
    wait_buffer.sem_flg = 0;
    // Perform the semaphore operation and check for errors
    if (semop(semid, &wait_buffer, 1) == -1)  {
        perror("semaphore_wait failed");
        exit(EXIT_FAILURE);
    }
}

void semaphore_signal(int semid, int semnumber) {
    // declare a sembuf
    struct sembuf signal_buffer;
    // We will perform an operation on the semnumber semaphore of semid
    signal_buffer.sem_num = semnumber;
    //We will add 1 to the semaphore
    signal_buffer.sem_op = 1;
    signal_buffer.sem_flg = 0;
    // Perform the semaphore operation and check for errors
    if (semop(semid, &signal_buffer, 1) == -1)  {
        perror("semaphore_signal failed");
        exit(EXIT_FAILURE);
    }
}

// Small wrapper functions to convert the keys of the shared memory
// and the semaphores to values.
// Used in 2013 Assignment 5 solutions.
int get_semid(key_t semkey) {
    int value = semget(semkey, NUMBER_OF_SEMAPHORES, 0777 | IPC_CREAT);
    if (value == -1) {
        perror("semget failed");
        exit(EXIT_FAILURE);
    }
    return value;
}

int get_shmid(key_t shmkey) {
    int value = shmget(shmkey, sizeof(struct shared_variable_struct), 0777 | IPC_CREAT);
    if (value == -1) {
        perror("shmkey failed");
        exit(EXIT_FAILURE);
    }
    return value;
}

int firstRequestAmt(int list[]) {
    return list[0];
}

void addEndOfList(int list[], int addition, int waitCnt) {
    list[waitCnt] = addition;
}

void deleteFirstRequest(int list[], int length) {
    int i = 0;
    for (i; i < (length - 1); i ++) {
        list[i] = list[i+1]; //shifts array to left so next request list[0] and so on
    }
    list[length-1] = 0;

}