#include "codexion.h"

int parser(int argc, char **argv) {
    int i;

    i = 1;
    if (argc != 9) {
        printf("Usage: codexion <num_coders> <burnout> <compile> <debug> "
                    "<refactor> <compiles> <dongle_cooldown> <scheduler>\n");
        return 1;
    }
    while (i <= 7) {
        if (!ft_atoi(argv[i]) || ft_atoi(argv[i]) < 0) {
            printf("<%s>: It needs to be a positive int\n", get_arg(i));
            return 1;
        }
        i++;
    }
    if (!ft_strcmp(argv[8], "EDF")) {
        return 2;
    }
    if (!ft_strcmp(argv[8], "FIFO")) {
        return 3;
    }
    printf("<SCHEDULER> needs to be FIFO or EDF\n");
    return 1;
}

