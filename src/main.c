#include "codexion.h"

int main(int argc, char *argv[]) {
    if (argc != 8) {
        fprintf(stderr, "Usage: codexion <num_coders> <burnout> <compile> <debug> "
                    "<refactor> <compiles> <cooldown> <scheduler>\n");
        return 1;
    }

    return 0;
}

