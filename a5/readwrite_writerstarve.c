#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef NUM_THREADS
#define NUM_THREADS 25
#endif

void *reader(void *arg);//thread function for readers
void *writer(void *arg);//thread function for writers
void semwait(sem_t *sem);//error-checked semaphore wait
void semsignal(sem_t *sem);//error-checked semaphore signal

int readcount = 0, randNum = 0;
sem_t wrt, mutex;
time_t t;

/**
 * @author Hayden Schmackpfeffer
 * Assignment 5 Posix Threading
 */
int main(int argc, char const *argv[]) {
    pthread_t threads[NUM_THREADS];
    int thread_data[NUM_THREADS]; //just needs to include thread #, since we don't have any thread specific data
    int errorCheck;//used to error check thread creation

    //seed the random number generator
    srand((unsigned int)time(&t));

    //initialize semaphores
    if (sem_init(&mutex, 0, (unsigned int)1) < 0
        || sem_init(&wrt, 0, (unsigned int)1) < 0) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }

    //randomly generate readers and writers
    for (int i = 0; i < NUM_THREADS; ++i) {
        void *thread_func;//the function to call

        thread_data[i] = i;
        randNum = rand();

        //~50-50 chance to create a reader or writer thread
        if ( (randNum % 2) == 0) {
            thread_func = reader;
        } else {
            thread_func = writer;//make this a deposit
        }
        if ((errorCheck = pthread_create(&threads[i], NULL, thread_func, &thread_data[i]))) {
            fprintf(stderr, "error: pthread_create, %d\n", errorCheck);
            return EXIT_FAILURE;
        }
        printf("Creating thread: %d\n", i);
    }

    //join all the threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        if ((errorCheck = pthread_join(threads[i], NULL))) {
            fprintf(stderr, "error: pthread_join, %d\n", errorCheck);
        }
    }
    return EXIT_SUCCESS;
}

void *reader(void *arg) {
    t = time (NULL); //get the time 
    int *thread_num = (int *)arg; //cast the args

    printf("---Thread %d entering reader. Timestamp is: %ld . \n", *thread_num, t);
    
    //wait for mutex to be signalled
    semwait(&mutex);
    readcount++; //signify a reader is going
    if (readcount == 1) {
        semwait(&wrt); //while readers are going, wait
    }
    semsignal(&mutex);  

    //READ
    printf("---Thread %d is **BEGINNING READING**.\n", *thread_num);
    sleep(2);
    printf("---Thread %d is **DONE READING**.\n", *thread_num);

    //wait (mutex);
    semwait(&mutex);

    readcount--;
    if (readcount == 0) { //if there are no readers, there is finally time to write
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

    semwait(&wrt); //wait until signalled by the reader

    //write
    printf("---Thread %d is **BEGINNING WRITING**. \n", *thread_num);
    sleep(2);
    printf("---Thread %d is **DONE WRITING**.\n", *thread_num);

    semsignal(&wrt); //signal other waiting writers

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