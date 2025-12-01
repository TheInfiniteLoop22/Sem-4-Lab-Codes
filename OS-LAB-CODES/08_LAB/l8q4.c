#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define CHAIRS 5      // Number of waiting chairs
#define CUSTOMERS 15  // Total customers

sem_t customers;     // Counts waiting customers
sem_t barber;        // Barber is ready or not
sem_t mutex;         // Mutex for accessing waiting room seats

int waiting = 0;     // Number of customers waiting

void *barber_func(void *arg)
{
    while (1)
    {
        sem_wait(&customers);  // Wait for a customer to arrive
        sem_wait(&mutex);      // Exclusive access to waiting count
        waiting--;             // One waiting customer goes for haircut
        sem_post(&barber);     // Barber is ready to cut hair
        sem_post(&mutex);
        printf("Barber is cutting hair\n");
        sleep(3);              // Simulate haircut time
    }
    return NULL;
}

void *customer_func(void *arg)
{
    int id = *(int *)arg;
    while (1)
    {
        sem_wait(&mutex);
        if (waiting < CHAIRS)
        {
            waiting++;         // Take a seat in the waiting room
            printf("Customer %d is waiting. Waiting customers: %d\n", id, waiting);
            sem_post(&customers); // Notify barber that customer arrived
            sem_post(&mutex);     // Release waiting room seats mutex
            sem_wait(&barber);    // Wait until barber is ready
            printf("Customer %d is getting a haircut\n", id);
            sleep(1);             // Simulate haircut being done
            break;                // Customer leaves after haircut
        }
        else
        {
            printf("Customer %d leaves (no waiting chairs available)\n", id);
            sem_post(&mutex);
            break;                // Leaves if no chair available
        }
    }
    return NULL;
}

int main()
{
    pthread_t btid, ctid[CUSTOMERS];
    int ids[CUSTOMERS];

    sem_init(&customers, 0, 0);
    sem_init(&barber, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_create(&btid, NULL, barber_func, NULL);

    for (int i = 0; i < CUSTOMERS; i++)
    {
        ids[i] = i + 1;
        pthread_create(&ctid[i], NULL, customer_func, &ids[i]);
        sleep(1); // Customers arrive one by one
    }

    for (int i = 0; i < CUSTOMERS; i++)
        pthread_join(ctid[i], NULL);

    // In real scenario barber runs infinitely, so here we just end program
    return 0;
}

