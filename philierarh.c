#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>



const int phil = 5; // число философов

int vremobeda[5]; // время приема пищи

pthread_mutex_t vilki[5]; // вилки для еды

pthread_t phi[5]; // потоки философы


void* eat(void *x)
{
    int vleft = *((int*)x);
	int vright;
    vright = (vleft + 1) % 5;
    
    while (1)
	{
        if ((vleft % 3) != 1)
		{
            pthread_mutex_lock(&vilki[vleft]);
            pthread_mutex_lock(&vilki[vright]);
        }
        else
		{
            pthread_mutex_lock(&vilki[vright]);
            pthread_mutex_lock(&vilki[vleft]);
        }
        
        printf("%d Начал и взял вилки %d and %d\n", vleft + 1, vleft, vright);

        sleep(vremobeda[vleft]);

        if (((vleft % 3) + 1) != 1)
		{
            pthread_mutex_unlock(&vilki[vleft]);
            pthread_mutex_unlock(&vilki[vright]);
        }
        else
		{
            pthread_mutex_unlock(&vilki[vright]);
            pthread_mutex_unlock(&vilki[vleft]);
        }
        printf("%d Закончил и положил вилки на место %d и %d\n", vleft + 1, vleft, vright);
    }
}

int main()
{
    int masphil[5] = {0, 1, 2, 3, 4};
    
    for (int i = 0; i < 5; i++) vremobeda[i] = 2;
    for (int i = 0; i < 5; i++) pthread_mutex_init(&vilki[i], NULL);
    for (int i = 0; i < 5; i++) pthread_create(&phi[i], NULL, eat, &masphil[i]);
    for (int i = 0; i < 5; i++) pthread_join(phi[i], NULL);

    printf("the end");

    return 0;    
}