#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>

#define Nplates 3 //per philosopher
#define Think 1
#define Eat 3
#define SemKey 1998

void grab_forks(int left_fork_id);
void put_away_forks(int left_fork_id);
void eat(int plates);
void think();
void phiLife();

char *philosophers[5] = {"Ph1", "Ph2", "Ph3", "Ph4", "Ph5"};
int phID, semID, pid, status;

int main()
{
    int i = 0;

    semID = semget(SemKey, 5, 755 | IPC_CREAT);

    if (semID == -1)
    {
        exit(1);
    }

    while (i <= 4)
        semctl(semID, i++, SETVAL, 1);
    i = 0;

    while (i <= 4)
    {
        pid = fork();
        if (pid == 0)
        {
            phID = i;
            phiLife();
            printf("[%s]: left the table.\n", philosophers[phID]);
            return 0;
        }
        else if (pid < 0)
        {
            kill(-2, SIGTERM);
            fprintf(stderr, "[%d]: fork() error.\n", pid);
            exit(1);
        }
        i++;
    }
    while (1)
    {
        pid = wait(&status);
        if (pid < 0)
            break;
    }
    i = 0;
    if (semctl(semID, 0, IPC_RMID, 1) < 0)
        fprintf(stderr, "semaphore error.\n");
    return 0;
}

void grab_forks(int left_fork_id)
{
    int right_fork_id = left_fork_id - 1;
    if (right_fork_id < 0)
    {
        right_fork_id = 4;
    }

    printf("[%s]: take left [%d] and right [%d] forks.\n", philosophers[phID], left_fork_id, right_fork_id);
    struct sembuf semfork[2] = {
        {right_fork_id, -1, 0},
        {left_fork_id, -1, 0}};
    semop(semID, semfork, 2);
}

void put_away_forks(int left_fork_id)
{
    int right_fork_id = left_fork_id - 1;

    if (right_fork_id < 0)
    {
        right_fork_id = 4;
    }

    printf("[%s]: puts away left [%d] and right [%d] forks.\n", philosophers[phID], left_fork_id, right_fork_id);
    struct sembuf semfork[2] = {
        {right_fork_id, 1, 0},
        {left_fork_id, 1, 0}};
    semop(semID, semfork, 2);
}

void think()
{
    printf("[%s]: is thinking.\n", philosophers[phID]);
    sleep(Think);
}

void eat(int plates)
{
    grab_forks(phID);
    printf("[%s]: is eating.\n", philosophers[phID]);
    sleep(Eat);
    printf("[%s]: ate his %d plate.\n", philosophers[phID], Nplates - plates);
    put_away_forks(phID);
}

void phiLife()
{
    printf("[%s]: arrives at the table. \n", philosophers[phID]);
    int plates = Nplates;
    int food = 0;
    while (plates != 0)
    {
        if (food)
        {
            eat(--plates);
            food = 0;
        }

        else
        {
            think();
            food = 1;
        }
    }
}