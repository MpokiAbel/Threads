#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct thread_arg
{
    int repeat_times;
    char *lyrics;
} thread_argument;

void *supporter(void *arg)
{
    thread_argument *str = (thread_argument *)arg;
    int i;
    int pid;
    pthread_t tid;
    pid = getpid();
    tid = pthread_self();

    for (i = 0; i < str->repeat_times; i++)
    {
        printf("Process %d Thread %x : %s \n", pid, (unsigned int)tid, str->lyrics);
    }
    return (void *)tid;
}

int main(int argc, char **argv)
{
    int i;
    int no_French;
    int no_English;
    pthread_t *tids;
    thread_argument *french;
    thread_argument *english;
    int nb_threads = 0;

    //Providing the lyrics arguments to the struct
    french->lyrics = "Allons enfants de la patrie";
    english->lyrics = "Swing low, sweet chariot";

    if (argc != 5)
    {
        fprintf(stderr, "usage : %s Number_of_French Repeat_Times Number_of_English Repeat_Times\n", argv[0]);
        exit(-1);
    }

    no_French = atoi(argv[1]);
    french->repeat_times = atoi(argv[2]);
    no_English = atoi(argv[3]);
    english->repeat_times = atoi(argv[4]);

    nb_threads = no_French + no_English;

    tids = malloc(nb_threads * sizeof(pthread_t));

    /* Create the threads for french */
    for (i = 0; i < no_French; i++)
    {
        pthread_create(&tids[i], NULL, supporter, french);
    }

    /* Create the threads for English */
    for (; i < nb_threads; i++)
    {
        pthread_create(&tids[i], NULL, supporter, english);
    }

    /* Wait until every thread ended */
    for (i = 0; i < nb_threads; i++)
    {
        pthread_join(tids[i], NULL);
    }

    free(tids);
    return EXIT_SUCCESS;
}