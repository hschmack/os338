/**
 * Author: Hayden Schmackpfeffer (hbs25)
 * EECS 338: Assignment 1
 * Due 9/17/15 
 */
#define _XOPEN_SOURCE // required for cuserid to work

#include <unistd.h> // fork()
#include <limits.h>  // Needed for HOST_NAME_MAX
#include <errno.h>  // EXIT_FAILURE, //EXIT_SUCCESS
#include <stdlib.h> //exit
#include <time.h>   // to get system time
#include <stdio.h> //printf
#include <sys/wait.h>

#define NUM_CHILDREN 4 
#define MAX_N 10
#define DELAY 1 //second

//Function Declarations
void child_proc(int bin_start, int child_number);
void child(int child_number);
void print_info(int child_number);
char* cuserid_wrapper();
static int bin_coefficient(int n, int r);
int n;

int main(int argc, char *argv[]) {
  n = 2;
  int i;
  int status = 0;
  pid_t child1_pid, child2_pid, child3_pid, child4_pid;

  child1_pid = fork();

  if (child1_pid == -1){
    perror("Error trying to fork child1");
    exit(EXIT_FAILURE);
  }
  else if (child1_pid == 0) {
    print_info(1);
    printf("(%d (%d - 2)) binomial coefficent computations of integers n=2, 3, 10, start now! \n", MAX_N, MAX_N);
  } else {
    //these are executing first because it's the parent process. Is this a big deal?
    child2_pid = fork();

    if (child2_pid == -1){
      perror("Error trying to fork child2");
      exit(EXIT_FAILURE);
    }
    else if (child2_pid == 0) {
      sleep(1);
      print_info(2);
      child_proc(2, 2);

    } else {
        waitpid(child1_pid, &status, 0);
        printf("child1 terminated\n");

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
          printf("child3 terminated\n");
        }
        waitpid(child2_pid, &status, 0);
        printf("child2 terminated\n");

        child4_pid = fork();

        if (child4_pid == -1){
          perror("Error trying to fork child4");
          exit(EXIT_FAILURE);
        } 
        else if (child4_pid == 0){
          sleep(1);
           print_info(4);
          system("ls -l");
        } else {
           waitpid(child4_pid, &status, 0);
           printf("child4 terminated\n");
        }   
    }
  }
/*
  while ((wpid = wait(&status)) > 0){ //this should wait for all children to be done
    printf("Exit status of %d was %d \n", (int)wpid, status);
  } 
*/

  return 0;
}

void child_proc(int bin_start, int child_number){
  //get start time
  sleep(2);
  int i;
  for (i = bin_start; i < 11; i += 2) {
    int result = bin_coefficient(i, i-2);
    printf("CHILD %d printing binary coefficient of %d and %d which is %d \n", child_number, i, i-2, result);
    sleep(2);
  }
  //print current time - start time before exiting
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

/**
 * Get the cuserid, return error if it doesnt work
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