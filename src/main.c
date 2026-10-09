#include "codexion.h"

int main(int argc, char **argv) {
    int operation;

    operation = parser(argc, argv);
    if (operation == 1) {
        return 1;
    }
    ft_showinfo(argc, argv);
    if (operation == 2) {
        //EDF
    }
    if (operation == 3) {
        //FIFO
    }
    return 0;
}

