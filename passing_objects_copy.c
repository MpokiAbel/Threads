#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define LOOPS 20

#define BUF_SIZE 4

sem_t sem_mutex;
sem_t fullCount;
sem_t emptyCount;
int in = 0;
int out = 0;

int buffer[BUF_SIZE];

void pushing(int val)
{
    /* TO DO */

    sem_wait(&emptyCount);
    sem_wait(&sem_mutex);

    buffer[in] = val;
    printf("pushing value %d\n", buffer[in]);
    in = (in + 1) % BUF_SIZE;
    sem_post(&sem_mutex);
    sem_post(&fullCount);
    /* NOTE: pay attention to where to put the call to printf to be
       sure it can be helpful for debugging */
}

int fetching(void)
{
    int val = 0;

    sem_wait(&fullCount);
    sem_wait(&sem_mutex);

    val = buffer[out];
    printf("\t feched value %d\n", buffer[out]);
    out = (out + 1) % BUF_SIZE;

    /* NOTE: pay attention to where to put the call to printf to be
       sure it can be helpful for debugging */

    sem_post(&sem_mutex);
    sem_post(&emptyCount);

    return val;
}

void *thread_generating(void *arg)
{
    int i = 0;
    int new_value = 0;

    for (i = 0; i < LOOPS; i++)
    {
        new_value = rand() % 10;
        pushing(new_value);
    }

    return NULL;
}

void *thread_using(void *arg)
{
    int i = 0;
    int value = 0;

    for (i = 0; i < LOOPS; i++)
    {
        value = fetching();
    }

    return NULL;
}

int main(void)
{
    sem_init(&sem_mutex, 0, 1);
    sem_init(&fullCount, 0, 0);
    sem_init(&emptyCount, 0, BUF_SIZE);
    pthread_t tids[2];
    int i = 0;

    struct timespec tt;
    clock_gettime(CLOCK_MONOTONIC, &tt);
    /* seed for the random number generator */
    srand(tt.tv_sec);

    if (pthread_create(&tids[0], NULL, thread_using, NULL) != 0)
    {
        fprintf(stderr, "Failed to create the using thread\n");
        return EXIT_FAILURE;
    }

    if (pthread_create(&tids[1], NULL, thread_generating, NULL) != 0)
    {
        fprintf(stderr, "Failed to create the generating thread\n");
        return EXIT_FAILURE;
    }

    /* Wait until every thread ended */
    for (i = 0; i < 2; i++)
    {
        pthread_join(tids[i], NULL);
    }

    return EXIT_SUCCESS;
}
