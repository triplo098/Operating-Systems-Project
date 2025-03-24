#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

pthread_mutex_t mutex;

sem_t s[5];

int get_rand_s(int min, int max)
{
    return (rand() % (max - min + 1) + min);
}

void think(int phil_id)
{

    int duration = get_rand_s(1000, 5000);
    printf("%d is thinking %dms\n", phil_id, duration);
    usleep(duration);
}

void *philosopher(void *_phil_id)
{

    int phil_id = (int)_phil_id;

    while (1)
    {

        // printf("%d is thinking\n", phil_id);
        think(phil_id);
        // take_forks(phil_id);
        // eat(phil_id);
        // put_forks(phil_id);
    }
}

int main(int argc, char *argv[])
{

    // Check if the number of philosophers is provided
    if (argc != 2)
    {
        printf("Usage: %s <number of philosophers>\n", argv[0]);
        return 1;
    }

    srand(time(NULL));

    // Number of philosophers
    int philosophers_num = strtol(argv[1], NULL, 10);
    printf("Philosophers: %d \n", philosophers_num);

    // Initialize threads identifiers
    pthread_t threads[philosophers_num];

    // Creating threads
    for (int phil_id = 0; phil_id < philosophers_num; phil_id++)
        pthread_create(&threads[phil_id], NULL, philosopher, (void *)phil_id);

    // Joining threads
    for (int phil_id = 0; phil_id < philosophers_num; phil_id++)
        pthread_join(threads[phil_id], NULL);

    return 0;
}