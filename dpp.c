#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>

// defining philosophers states
#define THINKING 0
#define HUNGRY 1
#define EATING 2

// mutex for critical section - taking and putting down forks
pthread_mutex_t cs_mutex = PTHREAD_MUTEX_INITIALIZER;


pthread_mutex_t *philosopher_forks; // array of mutexes, forks[i] means
                                    // the fork on the left and right of philosopher is available/unavailable

pthread_t *threads;
unsigned short *state;

int philosophers_num;


int get_rand(int min, int max)
{
    return (rand() % (max - min + 1) + min);
}

// tests if philosopher can eat, if he is hungry and both neighbors are not eating
void test(int phil_id)
{
    if (state[phil_id] == HUNGRY &&
        state[phil_id % philosophers_num] != EATING &&
        state[(phil_id + 1) % philosophers_num] != EATING)
    {
        state[phil_id] = EATING;
        pthread_mutex_unlock(&philosopher_forks[phil_id]);
    }
}

void put_forks(int phil_id)
{
    pthread_mutex_lock(&cs_mutex);
    state[phil_id] = THINKING;

    test((phil_id - 1) % philosophers_num);
    test((phil_id + 1) % philosophers_num);

    pthread_mutex_unlock(&cs_mutex);
}

void eat(int phil_id, unsigned long *ate_total)
{

    int duration = get_rand(1e3, 5e3);

    // uninterruptible print
    pthread_mutex_lock(&cs_mutex);

    printf("%d       ate       for   %lums\n", phil_id, *ate_total);
    printf("%d is going to eat for   %dms\n", phil_id, duration);

    pthread_mutex_unlock(&cs_mutex);

    *ate_total += duration;

    usleep(duration * 1000);
}

void take_forks(int phil_id)
{
    // enter critical section, uninterruptible state change and print
    pthread_mutex_lock(&cs_mutex);
    state[phil_id] = HUNGRY;
    printf("%d is hungry\n", phil_id);
    pthread_mutex_unlock(&cs_mutex);

    test(phil_id);

    pthread_mutex_lock(&philosopher_forks[phil_id]);
}

void think(int phil_id)
{
    int duration = get_rand(1e3, 5e3);

    // uninterruptible print
    pthread_mutex_lock(&cs_mutex);
    printf("%d is going to think for %dms\n", phil_id, duration);
    pthread_mutex_unlock(&cs_mutex);

    usleep(duration * 1000);
}

void *philosopher(void *_phil_id)
{
    // get the philosopher id
    int phil_id = *(int *)_phil_id;

    unsigned long ate_total = 0;

    // free the memory allocated for the argument
    free(_phil_id);

    while (1)
    {
        think(phil_id);
        take_forks(phil_id);
        eat(phil_id, &ate_total);
        put_forks(phil_id);
    }
}

void handle_interrupt(int sig) {
    printf("\n Interrupt caught: %d\n", sig);


    // for()
    

    exit(0);

}


int main(int argc, char *argv[])
{
    // Set the interrupt handler for SIGINT
    signal(SIGINT, handle_interrupt);

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


    // memory allocation for philosophers states and forks
    state = malloc(philosophers_num * sizeof(unsigned short));
    philosopher_forks = malloc(philosophers_num * sizeof(pthread_mutex_t));
    threads = malloc(philosophers_num * sizeof(pthread_t));

    for (int i = 0; i < philosophers_num; i++)
    {
        state[i] = THINKING;
        pthread_mutex_init(&philosopher_forks[i], 0);
    }
    

    // Creating threads
    for (int phil_id = 0; phil_id < philosophers_num; phil_id++)
    {
        int *arg = malloc(sizeof(int));
        *arg = phil_id;
        pthread_create(&threads[phil_id], NULL, philosopher, arg);
    }

    // Joining threads
    for (int phil_id = 0; phil_id < philosophers_num; phil_id++)
        pthread_join(threads[phil_id], NULL);


    free(state);
    free(philosopher_forks);
    free(threads);

    return 0;
}