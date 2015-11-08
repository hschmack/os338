#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef NUM_THREADS
#define NUM_THREADS 200
#endif

//the range of return values for getRandom() function is
//[-RAND_RANGE/2, RAND_RANGE/2]
const unsigned int RAND_RANGE = RAND_MAX>>10;

int getRand();//returns a random int
void *reader(void *arg);//thread function for readers
void *writer(void *arg);//thread function for writers
void semwait(sem_t *sem);//error-checked semaphore wait
void semsignal(sem_t *sem);//error-checked semaphore signal

int readcount = 0;
sem_t wrt, mutex;
time_t t;

int main(int argc, char const *argv[]) {
    pthread_t threads[NUM_THREADS];
    int thread_data[NUM_THREADS]; //just needs to include thread #, since we don't have any thread specific data
    //int errorCheck;//used to error check thread creation

    //seed the random number generator
    srand((unsigned int)time(&t));

    //initialize semaphores
    if (sem_init(&mutex, 0, (unsigned int)1) < 0
        || sem_init(&wrt, 0, (unsigned int)1) < 0) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }

    thread_data[0]= 0;
    pthread_create(&threads[0], NULL, reader, &thread_data[0]);

    thread_data[1]= 1;
    pthread_create(&threads[1], NULL, writer, &thread_data[1]);

    thread_data[2]= 2;
    pthread_create(&threads[2], NULL, reader, &thread_data[2]);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    pthread_join(threads[2], NULL);

    return EXIT_SUCCESS;
}

int getRand() {
    return ((rand() % RAND_RANGE) - RAND_RANGE/2);
}

void *reader(void *arg) {
    t = time (NULL); //get the time 
    int *thread_num = (int *)arg; //cast the args

    printf("---Thread %d entering reader. Timestamp is: %ld . \n", *thread_num, t);
    
    //wait mutex
    semwait(&mutex);
    readcount++;
    if (readcount == 1) {
        semwait(&wrt);
    }
    semsignal(&mutex);  
    //READ

    printf("---Thread %d is **BEGINNING READING**.\n", *thread_num);
    sleep(2);
    printf("---Thread %d is **DONE READING**.\n", *thread_num);

    //wait (mutex);
    semwait(&mutex);

    readcount--;
    if (readcount == 0) {
        semsignal(&wrt);
    }
    semsignal(&mutex);

    time_t t_end = time(NULL);
    printf("---Thread %d exiting reader. Timestamp is: %ld, elapsed time is %ld seconds.\n", *thread_num, t_end, (t_end - t));
    pthread_exit(NULL);
}

void *writer(void *arg) {
    t = time (NULL); //get the time 
    int *thread_num = (int *)arg; //cast the args

    printf("---Thread %d entering writer. Timestamp is: %ld \n", *thread_num, t);

    semwait(&wrt);

    //write
    printf("---Thread %d is **BEGINNING WRITING**. \n", *thread_num);
    sleep(2);
    printf("---Thread %d is **DONE WRITING**.\n", *thread_num);

    semsignal(&wrt);

    time_t t_end = time(NULL);
    printf("---Thread %d exiting writer. Timestamp is: %ld, elapsed time is %ld seconds.\n", *thread_num, t_end, (t_end - t));

    pthread_exit(NULL);
}
/*
*   Error-checked semaphore wait.
*/
void semwait(sem_t *sem) {
    if (sem_wait(sem) < 0) {
        perror("sem_wait");
        exit(EXIT_FAILURE);
    }
}

/*
*   Error-checked semaphore signal.
*/
void semsignal(sem_t *sem) {
    if (sem_post(sem) < 0) {
        perror("sem_post");
        exit(EXIT_FAILURE);
    }
}