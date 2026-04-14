#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>



float vremobeda[5] = {1, 1.5, 1, 1.5, 1.2}; // время приема пищи

pthread_mutex_t vilki[5]; // вилки для еды

pthread_t phi[5]; // потоки философы

sem_t obg;

void *obgora(void *x)
{
    int vleft = *((int*)x);
	int vright;
    vright = (vleft + 1) % 5;
    
    while (1)
	{
        pthread_mutex_lock(&vilki[vleft]);
        pthread_mutex_lock(&vilki[vright]);

        sem_post(&obg);
        sleep(vremobeda[vleft]);
        printf("%d Обжора начал есть\n", vleft + 1);
        
        sem_wait(&obg);
        
        printf("%d Поел\n", vleft + 1);
        
        pthread_mutex_unlock(&vilki[vleft]);
        pthread_mutex_unlock(&vilki[vright]);

        sleep(0.2);
    }
}

void *eat(void *x)
{
    int golod = 0;
    int vleft = *((int*)x);
	int vright;
    vright = (vleft + 1) % 5;
    int i;
    while (1)
	{
        pthread_mutex_lock(&vilki[vleft]);
        for(i = 0; i < 3; i++)
		{
            
            if(pthread_mutex_trylock(&vilki[vright]) == 0) break;
            sleep(0.5);
        }
        if(i == 3)
        {
            golod++;
            pthread_mutex_unlock(&vilki[vleft]);
            if (golod == 10) return NULL;
            sleep(2);
        }
        else
        {
		    sleep(vremobeda[vleft]);
		    golod = 0;
		    pthread_mutex_unlock(&vilki[vleft]);
		    pthread_mutex_unlock(&vilki[vright]);
		    sleep(vremobeda[vleft]);
        }

            printf("%d Не ел %d\n", vleft + 1, golod);
            
        }
}

int main()
{
    int masphil[5] = {0, 1, 2, 3, 4};
    
    for (int i = 0; i < 5; i++) pthread_mutex_init(&vilki[i], NULL);
    sem_init(&obg, 0, 0);

    pthread_create(&phi[0], NULL, obgora, &masphil[0]); 
    pthread_create(&phi[1], NULL, eat, &masphil[1]);
    pthread_create(&phi[2], NULL, obgora, &masphil[2]);
    pthread_create(&phi[3], NULL, eat, &masphil[3]);
    pthread_create(&phi[4], NULL, eat, &masphil[4]);

	sem_wait(&obg);
	
	pthread_join(phi[1], NULL);
    sleep(1);

    return 0;    
}
