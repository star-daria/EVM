#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>

pthread_mutex_t mu;
sem_t semaph, zelen_dver, krasn_dver;
int n, m;
int kol_patron;
int flag = 1;

void *th_gvard(void *x)
{
    int vistrel;
    int id_loc = *(int *)x;
    sem_post(&semaph);
    do
	{
        sem_wait(&zelen_dver);
        
        vistrel = rand() % kol_patron;

        pthread_mutex_lock(&mu);
        
        n--;
        
        if (vistrel != 0) m++;
        
        if (n == 0)  
		{
            n = m;
            for(int i = 0; i < n; i++) sem_post(&krasn_dver);
        }
        if (vistrel == 0)
		{
            printf("%d died\n", id_loc);
            pthread_mutex_unlock(&mu);
            return NULL;
        }
        pthread_mutex_unlock(&mu);
        
        sem_wait(&krasn_dver);
        
        pthread_mutex_lock(&mu);
        m--;
        if (m == 0)
		{
            for(int i = 0; i < n; i++) sem_post(&zelen_dver);
            if (n == 1) flag = 0;
        }
        pthread_mutex_unlock(&mu);
    }
	while(flag);
    printf("%d winner\n", id_loc);
    return NULL;
}

int main(void)
{
    int id, res;
    n = 20; m = 0; kol_patron = 8;
    
    pthread_t gvardeec[20];
    
    sem_init(&zelen_dver, 0, 20);
    sem_init(&krasn_dver, 0, 0);
    sem_init(&semaph, 0, 0);
    pthread_mutex_init(&mu, NULL);
    
    for(id = 1; id <= 20; id++)
	{
        res = pthread_create(&gvardeec[id - 1], NULL, th_gvard, &id);
        if(res != 0) return EXIT_FAILURE; 
        sem_wait(&semaph);
    }
    for(int i = 1; i <= 20; i++)
	{
        res = pthread_join(gvardeec[i-1], NULL);
        if(res != 0) return EXIT_FAILURE;
    }
    
    if(flag) printf("no winner\n");
    
    sem_destroy(&zelen_dver);
    sem_destroy(&krasn_dver);
    sem_destroy(&semaph);
    pthread_mutex_destroy(&mu);
    
    printf("the end\n");
    return 0;
}
