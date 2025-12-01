#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int buf[5], in = -1, out = -1;
sem_t mutex, full, empty, ahead;

void *produce(void *arg)
{
    int i;
    for(i=0;i<25;i++)
    {
        sem_wait(&ahead);          
        sem_wait(&empty);
        sem_wait(&mutex);
        printf("produced item is %d\n",i);
        buf[(++in)%5]=i;
        sem_post(&mutex);
        sem_post(&full);
        usleep(10000);            
    }
    return NULL;
}

void *consume(void *arg)
{
    int item, i;
    for(i=0;i<25;i++)
    {
        sem_wait(&full);
        sem_wait(&mutex);
        item=buf[(++out)%5];
        printf("CONSUMED item is %d\n",item);
        sem_post(&mutex);
        sem_post(&empty);
        sem_post(&ahead);          
        sleep(2);                  
    }
    return NULL;
}

int main()
{
    pthread_t tid1,tid2;
    sem_init(&mutex, 0, 1);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, 5);
    sem_init(&ahead, 0, 10); // this controls "at most 10 ahead"
    pthread_create(&tid1, NULL, produce, NULL);
    pthread_create(&tid2, NULL, consume, NULL);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    return 0;
}

