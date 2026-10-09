/* codexion.h */
#ifndef CODEXION_H
#define CODEXION_H

#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define NUM_CODERS      5
#define BURNOUT         100
#define COMPILE         200
#define DEBUG           100
#define REFACTOR        150
#define NUM_COMPILES    3
#define DONGLE_COOLDOWN 100
#define SCHEDULER       "EDF" // FIFO

typedef struct s_Coder {
    int     id;
    int     num_compiles;
    time_t  last_compiler_start;
    time_t  last_compile_end;
    time_t  last_debug_start;
    time_t  last_refactor_start;
    time_t  last_compile_finish;
    time_t  burnout_time;
    bool    is_alive;
    int     dongle_cooldown_remaining;
} t_Coder;

typedef struct s_Dongle {
    int     id;
    time_t  last_released;
    time_t  cooldown_until;
    int     is_available;
} t_Dongle;

typedef struct s_Scheduler {
    int num_coder;
    int num_dongles;
    int time_to_burnout;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int time_to_refactor_required;
    int dongle_cooldown;
} t_Scheduler;

typedef struct t_Engine {
    t_Coder     coder_array[1000];
    t_Dongle    dongle_array[1000];
    t_Scheduler config;
    time_t      simulation_start;
    time_t      simulation_end;
    int         num_coders_alive;
    int         total_compiles;
} t_Engine;

typedef struct  s_PriorityQueue {
    t_Coder*    elements;
    int         size;
    int         capacity;
} t_PriorityQueue;

// Util Function
int	ft_atoi(const char *s);
int parser(int argc, char **argv);
int	ft_strcmp(const char *s1, const char *s2);
bool is_valid_int(const char *str);
void ft_showinfo(int argc, char **argv);
const char *get_arg(int index);

#endif

