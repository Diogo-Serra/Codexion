#include "stdio.h"
#include "pthread.h"


void *computation(void *add) {
    long *add_num = (long *) (add);
    long sum = 0;
    int i;

    i = 0;
    while (i < 1000000000) {
        sum += *add_num;
        i++;
    }
    printf("Add: %ld\n", *add_num);
    return NULL;
}

int main() {
    pthread_t thread1;
    pthread_t thread2;

    long value1 = 1;
    long value2 = 2;

    /*
    computation((void *) &value1);
    computation((void *) &value2);
    */

    pthread_create(&thread1, NULL, computation, (void *) &value1);
    pthread_create(&thread2, NULL, computation, (void *) &value2);
    
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return(0);
}
