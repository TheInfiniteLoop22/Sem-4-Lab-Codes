#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t mutex, wrt;
int readcount = 0;
int data = 0;

void *reader(void *arg)
{
    int id = *(int *)arg;
    while(1)
    {
        sem_wait(&mutex);
        readcount++;
        if(readcount == 1)
            sem_wait(&wrt);
        sem_post(&mutex);

        // Reading section
        printf("Reader %d: read data = %d\n", id, data);
        sleep(1);

        sem_wait(&mutex);
        readcount--;
        if(readcount == 0)
            sem_post(&wrt);
        sem_post(&mutex);

        sleep(1);
    }
    return NULL;
}

void *writer(void *arg)
{
    int id = *(int *)arg;
    while(1)
    {
        sem_wait(&wrt);
        // Writing section
        data++;
        printf("Writer %d: wrote data = %d\n", id, data);
        sleep(2);
        sem_post(&wrt);

        sleep(2);
    }
    return NULL;
}

int main()
{
    pthread_t rtid[3], wtid[2];
    int i;
    int rid[3] = {1,2,3}, wid[2] = {1,2};

    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);

    for(i=0;i<3;i++)
        pthread_create(&rtid[i], NULL, reader, &rid[i]);
    for(i=0;i<2;i++)
        pthread_create(&wtid[i], NULL, writer, &wid[i]);

    for(i=0;i<3;i++)
        pthread_join(rtid[i], NULL);
    for(i=0;i<2;i++)
        pthread_join(wtid[i], NULL);

    return 0;
}

