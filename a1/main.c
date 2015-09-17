/**
 * Author: Hayden Schmackpfeffer (hbs25)
 * EECS 338: Assignment 1
 * Due 9/17/15 
 */

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
static void child(int child_number);
static int bin_coefficient(int n, int r);
int n;

int main(int argc, char *argv[]) {
  n = 2;
  int i;
  int status = 0;
  pid_t pid, wpid;

  for (i =0; i < NUM_CHILDREN; i++){
    if(i == 2) { sleep(1);}
    pid = fork();
    if(pid < 0) {
        printf("Error");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        printf("Child (%d): %d\n", i + 1, getpid());
        child(i+1);
        exit(EXIT_SUCCESS); 
    } else  {
        wait(NULL);
    }
  }
/*
  while ((wpid = wait(&status)) > 0){ //this should wait for all children to be done
    printf("Exit status of %d was %d \n", (int)wpid, status);
  } 
*/

  return 0;
}

static void child(int child_number){

  if(child_number == 1){
    printf("(%d (%d - 2)) binomial coefficent computations of integers n=2, 3, 10, start now! \n", MAX_N, MAX_N);
  } else if (child_number == 2){
    printf("INSIDE CHILD %d\n", child_number);
    int i;
    for(i = 2; i <= MAX_N; i += 2){
      int result = bin_coefficient(i, i-2);
      printf("CHILD %d printing binary coefficient of %d and %d which is %d \n", child_number, i, i-2, result);
      sleep(1);
    }

  } else if (child_number == 3){
    printf("INSIDE CHILD: %d\n", child_number);
    int i;
    for(i = 3; i <= MAX_N - 1 ; i += 2){
      int result = bin_coefficient(i, i-2);
      printf("CHILD %d printing binary coefficient of %d and %d which is %d \n", child_number, i, i-2, result);
      sleep(1);
    }
  } else if (child_number == 4){
    printf("INSIDE CHILD= %d\n", child_number);
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