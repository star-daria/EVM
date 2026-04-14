#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

sem_t bak;
sem_t ed;
pthread_mutex_t mu;
int bul = 6; 
int a[2]; 
int n = sizeof(a)/sizeof(int);
int b = 0;
int k = 0;
int s = 0;
int v;
void inp(int m) // помещение 
{
	a[b] = m;
	b = (b + 1) % n;
}
int take() //извлечение
{
	int j;
	j = a[k];
	a[k] = 0;
	k = (k + 1) % n;
	return j;
}

void *pekar(void *x) //пекарь
{
	while(1)
	{
	    if (s >= bul)
		{
		    return NULL;
		}
		sleep(0.5);
		sem_wait(&bak);
		pthread_mutex_lock(&mu);
		inp(++s);
		for(int i = 0; i < n; i++)
		{
		    printf("%d ", a[i]);
		}
		printf(" Испек: %d\n", s);
		pthread_mutex_unlock(&mu);
		sem_post(&ed);
	}
}

void *edok(void *x) // едок
{
    while (1)
    {
        if (s >= bul)
        {
		    return NULL;
		}
        sem_wait(&ed);
        pthread_mutex_lock(&mu);
        v = take();
        for(int i = 0; i < n; i++)
		{
		    printf("%d ", a[i]);
		}
        sleep(1);
        printf(" Съедено: %d\n", v);
        pthread_mutex_unlock(&mu);
        sem_post(&bak);
    }
}

int main(void)
{
	int y = 2, z = 2;
	
	pthread_mutex_init(&mu, NULL);
	
	sem_init(&bak, 0, n);
	sem_init(&ed, 0, 0);
	
	pthread_t pek[y];
	pthread_t edo[z];
	
	for (int i = 0; i < y; i++) pthread_create(&pek[i],NULL, pekar, NULL);
	for (int i = 0; i < z; i++) pthread_create(&edo[i],NULL, edok, NULL);
	for (int i = 0; i < y; i++) pthread_join(pek[i], NULL);
	for (int i = 0; i < z; i++) pthread_join(edo[i], NULL);

    printf("the end");
    
	return 0;
}




