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

#define INTERP_BASE 10
#define INTERVAL_S 0
#define INTERVAL_US 500000

int counter  = 0;
int seconds = 0;
int ticks = 0;

void handler(int sigalarm);
int numcheck(const char* str);

int main(int argc, char const *argv[]) {
    /*Define Vairables*/
    int i;
    char* end;

    /*Vaildate Input*/
    if((argc != 2)){
        printf("usage: ./timeit <seconds>\n");
        exit(EXIT_FAILURE);
    }
    if(!(numcheck(argv[1]))){
        printf("%s: malformed time.\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    seconds = strtol(argv[1], &end, INTERP_BASE);
    ticks = seconds*2;


    /*Set up signal Handler*/
    struct sigaction sa;
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
    struct itimerval it;
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

int numcheck(const char* str){
    int i = 0;
    while(str[i] != '\0'){
        if((str[i] < '0') || (str[i] > '9')){
            return 0;
        }
        i++;
    }
    return 1;
}

