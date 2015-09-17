/**
 * Author: Hayden Schmackpfeffer (hbs25)
 * EECS 338: Assignment 1
 * Due 9/17/15 
 */
#define _XOPEN_SOURCE // required for cuserid to work

#include <unistd.h>   // fork()
#include <limits.h>   // Needed for HOST_NAME_MAX
#include <errno.h>    // EXIT_FAILURE, //EXIT_SUCCESS
#include <stdlib.h>   // exit
#include <time.h>     // to get system time
#include <stdio.h>    // printf
#include <sys/wait.h> // 
#include <sys/time.h>
#include <sys/resource.h>

#define NUM_CHILDREN 4 
#define MAX_N 10
#define DELAY 1 //second

//Function Declarations
void child_proc(int bin_start, int child_number);
void child(int child_number);
void print_info(int child_number);
void print_time_info(int child_number);
char* cuserid_wrapper();
void getrusage_wrapper(struct rusage* output);
void waste_time(); 
static int bin_coefficient(int n, int r);

int main(int argc, char *argv[]) {
  int status = 0;
  pid_t child1_pid, child2_pid, child3_pid, child4_pid, wpid;

  child1_pid = fork();

  if (child1_pid == -1){
    perror("Error trying to fork child1");
    exit(EXIT_FAILURE);
  }
  else if (child1_pid == 0) {
    print_info(1);
    printf("(%d (%d - 2)) binomial coefficent computations of integers n=2, 3, 10, start now! \n", MAX_N, MAX_N);
    print_time_info(1);
  } else { //if here then we're the parent, execute child 2
    child2_pid = fork();

    if (child2_pid == -1){
      perror("Error trying to fork child2");
      exit(EXIT_FAILURE);
    }
    else if (child2_pid == 0) {
      sleep(1);
      print_info(2);
      child_proc(2, 2);

    } else { //if here then we're the parent, execute child 3
        waitpid(child1_pid, &status, 0);
        printf("child1 terminated with status: %d\n", status);

        child3_pid = fork();

        if (child3_pid == -1){
          perror("Error trying to fork child3");
          exit(EXIT_FAILURE);
        }
        else if (child3_pid == 0) {
          sleep(2);
          print_info(3);
          child_proc(3, 3);

        } else {
          waitpid(child3_pid, &status, 0);
          printf("child3 terminated with status: %d\n", status);
        }
        waitpid(child2_pid, &status, 0);
        printf("child2 terminated with status: %d\n", status);

        child4_pid = fork();

        if (child4_pid == -1){
          perror("Error trying to fork child4");
          exit(EXIT_FAILURE);
        } 
        else if (child4_pid == 0){
          sleep(1);
           print_info(4);
          system("ls -l");
          print_time_info(4);
        } else {
           waitpid(child4_pid, &status, 0);
           printf("child4 terminated with status: %d\n", status);
           print_time_info(0);
        }   
    }
  }
  return (EXIT_SUCCESS);
}

void child_proc(int bin_start, int child_number){
  sleep(2);
  int i;
  for (i = bin_start; i < 11; i += 2) {
    int result = bin_coefficient(i, i-2);
    printf("[CHILD %d] printing binary coefficient of %d and %d which is %d \n", child_number, i, i-2, result);
    sleep(2);
  }
  print_time_info(child_number);
  exit(EXIT_SUCCESS);
}

void print_info(int child_number){
  printf("-------------------------------------------\n");
  printf("[CHILD %d] PPID = %d, PID = %d\n", child_number, getppid(), getpid());
  printf("The username is: %s\n", cuserid_wrapper());

  printf("user id: %d\n", getuid()); 
  printf("effective user id: %d\n", geteuid());
  printf("group id: %d\n",  getgid());
  printf("effective group id: %d\n", getegid());
  printf("-------------------------------------------\n");
}

void print_time_info(int child_number){
  time_t current_time;
  waste_time();

  struct rusage res_usage;

  current_time = time(NULL);
  if (current_time == ((time_t) -1 )){
    perror("Error getting time");
    exit(EXIT_FAILURE);
  }

  getrusage_wrapper(&res_usage);
  if( child_number > 0 ) {
    printf("[Child %d] user CPU time: %d us\n", child_number, (int)res_usage.ru_utime.tv_usec);
    printf("[Child %d] system CPU time: %d us\n", child_number, (int)res_usage.ru_stime.tv_usec);
  }
  else {
    printf("[Parent] user CPU time: %d us\n",  (int)res_usage.ru_utime.tv_usec);
    printf("[Parent] system CPU time: %d us\n",(int)res_usage.ru_stime.tv_usec);
  }
}

/**
 * Get the cuserid, return error if it doesnt work
 * Taken from Spring 2015 assignment 1
 */
char* cuserid_wrapper(){
    char* val = cuserid(NULL);
    if (val == NULL) {
        perror("error getting cuserid");
        exit(errno);
    } else {
        return val;
    }
}

/**
 * For setting the rusage struct and detecting if its an error
 * Taken from Spring 2015 assignment 1
 */ 
void getrusage_wrapper(struct rusage* output){
    int val = getrusage(RUSAGE_SELF, output);
    if (val == -1){
        perror("getrusage_wrapper");
        exit(EXIT_FAILURE);
    }
}


void waste_time(){
    int i;
    int j = 0;
    printf("Wasting some CPU time to get significant results\n");
    for(i = 0; i < 100000000; i++){
	    //waste some CPU Time to get significant results for execution time
      j += 1;
    }
}
/*
 * computes binomial coefficents of n and r
 * Taken from: http://stackoverflow.com/questions/24294192/computing-the-binomial-coefficient-in-c
 */
static int bin_coefficient(int n,int r)
{
    int ans=1;
    r=r>n-r?n-r:r;
    int j=1;
    for(;j<=r;j++,n--)
    {
        if(n%j==0)
        {
            ans*=n/j;
        }else
        if(ans%j==0)
        {
            ans=ans/j*n;
        }else
        {
            ans=(ans*n)/j;
        }
    }
    return ans;
}
