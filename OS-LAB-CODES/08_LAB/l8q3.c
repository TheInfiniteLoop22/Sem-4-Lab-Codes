#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t semA, semB;

void *thread1(void *arg)
{
    sem_wait(&semA);
    printf("Thread 1 acquired semA\n");
    sleep(1);  // simulate some work

    printf("Thread 1 trying to acquire semB\n");
    sem_wait(&semB);  // BLOCKS - waiting for semB which thread2 holds

    printf("Thread 1 acquired semB\n");

    sem_post(&semB);
    sem_post(&semA);
    return NULL;
}

void *thread2(void *arg)
{
    sem_wait(&semB);
    printf("Thread 2 acquired semB\n");
    sleep(1);  // simulate some work

    printf("Thread 2 trying to acquire semA\n");
    sem_wait(&semA);  // BLOCKS - waiting for semA which thread1 holds

    printf("Thread 2 acquired semA\n");

    sem_post(&semA);
    sem_post(&semB);
    return NULL;
}

int main()
{
    pthread_t t1, t2;

    sem_init(&semA, 0, 1);  // initialized to 1 (available)
    sem_init(&semB, 0, 1);

    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&semA);
    sem_destroy(&semB);

    return 0;
}

