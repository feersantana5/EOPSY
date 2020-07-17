#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define Nplates 3
#define THINKING 1
#define HUNGRY 2
#define EATING 3
#define N 5
#define LEFT (i + N - 1) % N
#define RIGHT (i + 1) % N

pthread_mutex_t m;    //initialized to 1
int state[N];         //initiated to THINKING's
pthread_mutex_t s[N]; //initialized to 0's
pthread_t p[N];

void grab_forks(int i);
void put_away_forks(int i);
void test(int i);
void *phiLife(int i);

void up(pthread_mutex_t *m)
{
    pthread_mutex_unlock(m);
}
void down(pthread_mutex_t *m)

{
    pthread_mutex_lock(m);
}

int main()
{
    pthread_mutex_init(&m, NULL);
    up(&m);
    int i;
    for (i = 0; i < N; ++i)
    {
        p[i] = THINKING;
        pthread_mutex_init(&s[i], NULL);
        down(&s[i]);
    }
    for (i = 0; i < N; ++i)
    {
        pthread_create(&p[i], NULL, (void *)phiLife, (void *)i);
    }

    for (i = 0; i < N; ++i)
    {
        pthread_join(p[i], NULL);
    }
    for (i = 0; i < N; ++i)
    {
        pthread_mutex_destroy(&s[i

        ]);
    }
}

void grab_forks(int i)
{
    down(&m);
    state[i] = HUNGRY;
    test(i);
    up(&m);
    down(&s[i]);
}

void put_away_forks(int i)
{
    down(&m);
    state[i] = THINKING;
    test(LEFT);
    test(RIGHT);
    up(&m);
}

void test(int i)
{
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        state[i] = EATING;
        up(&s[i]);
    }
}

void *phiLife(int i)
{

    printf("[Ph%d]: arrives at the table.\n", i);
    for (int j = 0; j < Nplates; j++)
    {
        printf("[Ph%d]: is thinking.\n", i);
        sleep(2);
        grab_forks(i);
        printf("[Ph%d]: is eating.\n", i);
        sleep(3);
        put_away_forks(i);
        printf("[Ph%d]: finished his plate, has %d plates left.\n", i, (Nplates -j-1));
    }
    printf("[Ph%d]: left the table.\n", i);
}