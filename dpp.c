#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

// defining philosophers states
#define THINKING 0
#define HUNGRY 1
#define EATING 2

// mutex for critical section - taking and putting down forks
pthread_mutex_t cs_mutex = PTHREAD_MUTEX_INITIALIZER;

unsigned short* state;
pthread_mutex_t* forks;




int philosophers_num;

int get_rand(int min, int max)
{
    return (rand() % (max - min + 1) + min);
}

void test(int phil_id)
{

    if (state[phil_id] == HUNGRY &&
        state[phil_id % philosophers_num] != EATING && state[(phil_id + 1) % philosophers_num] != EATING)
    {
        state[phil_id] = EATING;
        pthread_mutex_unlock(&forks[phil_id]);
    }
}

void put_forks(int phil_id)
{
    pthread_mutex_lock(&cs_mutex);
    state[phil_id] = THINKING;
    pthread_mutex_unlock(&cs_mutex);

    test((phil_id - 1) % philosophers_num);
    test((phil_id + 1) % philosophers_num);
}

void eat(int phil_id)
{

    int duration = get_rand(1e3, 5e3);

    pthread_mutex_lock(&cs_mutex);
    printf("%d is going to eat for   %dms\n", phil_id, duration);
    pthread_mutex_unlock(&cs_mutex);

    usleep(duration * 1000);
}

void take_forks(int phil_id)
{
    pthread_mutex_lock(&cs_mutex);
    state[phil_id] = HUNGRY;
    printf("%d is hungry\n", phil_id);
    pthread_mutex_unlock(&cs_mutex);

    test(phil_id);

    pthread_mutex_lock(&forks[phil_id]);
}

void think(int phil_id)
{
    pthread_mutex_lock(&cs_mutex);

    state[phil_id] = THINKING;
    int duration = get_rand(1e3, 5e3);
    printf("%d is going to think for %dms\n", phil_id, duration);
    pthread_mutex_unlock(&cs_mutex);

    usleep(duration * 1000);
}

void *philosopher(void *_phil_id)
{
    // get the philosopher id
    int phil_id = *(int *)_phil_id;
    
    // free the memory allocated for the argument
    free(_phil_id);

    while (1)
    {
        // printf("%d is thinking\n", phil_id);
        think(phil_id);
        take_forks(phil_id);
        eat(phil_id);
        put_forks(phil_id);
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
    philosophers_num = strtol(argv[1], NULL, 10);
    printf("Philosophers: %d \n", philosophers_num);


    state = malloc(philosophers_num * sizeof(unsigned short));
    forks = malloc(philosophers_num * sizeof(pthread_mutex_t));


    for (int i = 0; i < philosophers_num; i++)
    {
        state[i] = THINKING;
        pthread_mutex_init(&forks[i], 0);        
    }
    // Initialize threads identifiers
    pthread_t threads[philosophers_num];

    // Creating threads
    for (int phil_id = 0; phil_id < philosophers_num; phil_id++) {
        int *arg = malloc(sizeof(int));
        *arg = phil_id;
        pthread_create(&threads[phil_id], NULL, philosopher, arg);
    }

    // Joining threads
    for (int phil_id = 0; phil_id < philosophers_num; phil_id++)
        pthread_join(threads[phil_id], NULL);

    free(state);
    free(forks);
    

    return 0;
}