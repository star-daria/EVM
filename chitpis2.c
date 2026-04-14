#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h> 

#define N 10 //читатели кол-во
#define M 5 //писатели кол-во

pthread_mutex_t bib;

pthread_t pisatel[M];
pthread_t chitatel[N];

sem_t sem_chit;
sem_t sem_pis;

int bibl = 0;
double cht = 1.2;
double pis = 1;
double chtp = 1;
double pisp = 5;
   
int queue = 0;

void *chitat(void *x)
{
    int id = *(int*)x;

    while(1)
	{
        while(1)
		{
            pthread_mutex_lock(&bib);
            if ((bibl != -1) && (queue == 0))
			{
                bibl+=1;
                pthread_mutex_unlock(&bib);
                break;
            }
            pthread_mutex_unlock(&bib);
            sem_wait(&sem_chit);
        }
        
        sem_post(&sem_chit);
        
        printf("Читатель %d читает\n", id);
        sleep(rand() % 2);
        sleep(cht);
        printf("Читатель %d прочитал\n", id);

        pthread_mutex_lock(&bib);
        bibl--;
        if (bibl == 0) sem_post(&sem_pis);
        pthread_mutex_unlock(&bib);
        sleep(chtp);
    }
    return NULL;
}

void *pisat(void *x)
{
    int id = *(int*)x;

    while(1)
	{
        while(1)
		{
            pthread_mutex_lock(&bib);
            if (bibl == 0)
			{
                bibl = -1;
                pthread_mutex_unlock(&bib);
                break;
            }
            queue++;
            pthread_mutex_unlock(&bib);
            sem_wait(&sem_pis);
            queue--;
        }
        printf(" Очередь %d \n", queue);
        printf("Писатель %d пишет\n", id);
        sleep(rand() % 3);
        sleep(pis);
        printf("Писатель %d написал\n", id);
        
        pthread_mutex_lock(&bib);
        bibl = 0;
        pthread_mutex_unlock(&bib);

        sem_post(&sem_pis);
        sem_post(&sem_chit);
        sleep(pisp);
    }
    return NULL;
}

int main()
{
    int chitm[N];
    
    int pistm[M];
    
	for(int i = 0; i < N; i++) chitm[i] = i + 1;
	
    for(int i = 0; i < M; i++) pistm[i] = i + 1;
    
    sem_init(&sem_chit, 0, 0);
    sem_init(&sem_pis, 0, 0);
    
    pthread_mutex_init(&bib, NULL);
    
    for (int i = 0; i < N; i++)
	{
        pthread_create(&chitatel[i], NULL, chitat, &chitm[i]);
        sleep(0.1);
    }
    
    for (int i = 0; i < M; i++)
	{
        pthread_create(&pisatel[i], NULL, pisat, &pistm[i]);
        sleep(0.1);
    }
    
    pthread_join(chitatel[0], NULL);

    return 0;
}
