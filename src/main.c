#include "stdio.h"

typedef struct s_Box {
    int x;
}   t_box;

t_box data(t_box x) {
 x.x = 1;
 return x;
}

int main() {
 t_box box;
 t_box ex;

 box.x = 0;
 printf("Hello World\n%d\n", box.x);
 ex = data(box);
 printf("%d\n", ex.x);
 return(0);
 }
