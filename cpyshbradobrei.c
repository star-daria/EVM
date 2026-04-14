#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

sem_t klient;
sem_t brad;
pthread_mutex_t mu; 
int stul = 5;
int m = 10; 
double vr_str = 1.1;
int s = 0;
int n = 0;
int t = 0;
int mas[5];

void *bradobr(void *x)
{
    while(1)
	{
        sem_wait(&brad);
        
        pthread_mutex_lock(&mu);
        
        if (n > 0)
		{
            n -= 1;
			s = (s + 1) % stul;
            pthread_mutex_unlock(&mu);
            sleep(vr_str);
            sem_post(&klient);
        }
        else pthread_mutex_unlock(&mu);
        if (t == m - 1) return NULL; 
    }
}

void *forklient(void *x)
{
    int a;
    pthread_mutex_lock(&mu);
    a = *(int *)x;

    if (n < stul)
	{
        mas[(n + s) % stul] = a;
		n += 1;
        printf("%d Ждет\n", a);
        
        pthread_mutex_unlock(&mu);
        sem_post(&brad);
        sem_wait(&klient);
        printf("Пострижен %d\n", a);
    }
    else
	{
        printf("%d Не нашел себе кресла\n", a);
        pthread_mutex_unlock(&mu);
    }
    
    t++;
}

int main()
{
    float wait[10] = {1, 1, 2, 1, 2, 1, 1, 2, 2, 1};
    
    pthread_t b;
    pthread_t k[m];
    
    sem_init(&klient, 0, 0);
    sem_init(&brad, 0, 0);
    
    pthread_mutex_init(&mu, NULL);

    pthread_create(&b, NULL, bradobr, &mas);
    for(int i = 0; i < m; i++)
	{
        pthread_create(&k[i], NULL, forklient, &i);
        sleep(wait[i]);
    }
    
    pthread_join(b, NULL);
    
    sleep(1);
    
    return 0;
}
