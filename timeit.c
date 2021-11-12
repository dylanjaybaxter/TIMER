/*
CPE 357 Lab 6
Author: Dylan Baxter (dybaxter)
File: timeit.c
Description: This file contains main functionality for a timer function
*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

/*Define Vaiables*/
/*Base of 0 for dynamic interpretation*/
#define INTERP_BASE 0
#define INTERVAL_S 0
#define INTERVAL_US 500000

int counter  = 0;
int seconds = 0;
int ticks = 0;

void handler(int sigalarm);

int main(int argc, char const *argv[]) {
    /*Define Variables*/
    int i;
    char* end;
    struct sigaction sa;
    struct itimerval it;

    /*Vaildate Number of Inputs*/
    if((argc != 2)){
        printf("usage: ./timeit <seconds>\n");
        exit(EXIT_FAILURE);
    }
    /*Interpret and Validate Input*/
    seconds = strtol(argv[1], &end, INTERP_BASE);
    if((*end != '\0')){
        printf("%s: malformed time.\n", argv[1]);
        printf("usage: ./timeit <seconds>\n");
        exit(EXIT_FAILURE);
    }
    if(seconds < 0){
        printf("Invalid time(%s).  Must be >= 0\n", argv[1]);
        printf("usage: ./timeit <seconds>\n");
        exit(EXIT_FAILURE);
    }
    if(seconds == 0){
        printf("Time's up!\n");
        return 0;
    }
    ticks = seconds*2;

    /*Set up signal Handler*/
    sa.sa_handler = handler;
    if(-1 == sigemptyset(&sa.sa_mask)){
        perror("SIGEMPTYSET");
        exit(EXIT_FAILURE);
    }
    sa.sa_flags = 0;
    if(-1 == sigaction(SIGALRM, &sa, NULL)){
        perror("SIGACTION");
        exit(EXIT_FAILURE);
    }

    /*Set up timer*/
    it.it_interval.tv_sec =INTERVAL_S;
    it.it_interval.tv_usec = INTERVAL_US;
    it.it_value = it.it_interval;
    setitimer(ITIMER_REAL, &it, NULL);

    /*Wait while timer executes*/
    for(i=0;i<ticks;i++){
        pause();
    }
    return 0;
}

/*Handler to be executed on alarm signal*/
void handler(int sigalarm){
    counter++;
    if(counter%2){
        printf("Tick...");
    }else{
        printf("Tock\n");
    }
    if(counter == ticks){
        printf("Times up!\n");
    }
    fflush(stdout);
}
