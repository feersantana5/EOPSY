#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#define NUM_CHILD 10  /*Number of child processes*/
#define WITH_SIGNALS /*Prepocessor code*/

#ifdef WITH_SIGNALS
int global_flag = 0;

void sigterm_handler() //3.a.child.
{
    printf("Child[%d]: recived SIGTERM signal, ending process.\n", getpid());
    exit(0);
}

void signal_handler() //3.b.parent.
{
    printf("\n");
    printf("Keyboard interrupt recieved!!!\n");
    printf("\n");
    global_flag = 1; //3.c.parent.
}

#endif

int main()
{
    pid_t pid, wpid;
    int id, i, killcode, status;
    int childs[NUM_CHILD], childs_ecodes[NUM_CHILD];

    pid = getpid();
    printf("The parent is: [%d].\n", pid);
    printf("\n");

#ifdef WITH_SIGNALS //3.a.parent.
    for (int p = 0; p < _NSIG; p++)
    {
        signal(p, SIG_IGN); //All signals ignored
    }
    signal(SIGCHLD, SIG_DFL);       //3.a.parent.
    signal(SIGINT, signal_handler); //3.b.parent.

#endif
    for (i = 0; i < NUM_CHILD; i++)
    {
#ifdef WITH_SIGNALS
        if (global_flag == 1) //3.c.parent.
        {
            printf("Interruption of Child Process creation!.\n");

            for (int j = 0; j <= i; j++) /*Killing all the created child processes*/
            {
                printf("Parent[%d]: sending SIGTERM signal.\n", getppid());
                kill(childs[j], SIGTERM);
            }

            break;
        }

#endif

        id = fork(); /* Storing all the created childs */
        childs[i] = id;

        if (id > 0) /* Parent Process */
        {
            sleep(1); /*1 Second delays between fork calls*/
        }

        else if (id == 0) /* Child Process */
        {
#ifdef WITH_SIGNALS
            signal(SIGINT, SIG_IGN);
            signal(SIGTERM, sigterm_handler);
#endif

            pid = getpid();

            /*Here the child sleeps for 10 secods*/
            printf("Parent[%d], created Child [%d].\n", getppid(), pid);
            sleep(10);
            exit(0);
        }

        else /* Fork creation error */
        {
            fprintf(stderr, "[%d] error: fork() failed.\n", pid);

            for (int j = 0; j <= i; j++) /*Killing all the created child processes*/
            {
                printf("Parent[%d]: sending SIGTERM signal.\n", getppid());
                killcode = kill(childs[j], SIGTERM);
                printf("Child[%d]: recived SIGTERM signal, terminating.\n", childs[j]);

                if (killcode == 0)
                {
                    printf("Child[%d] terminated successfully. \n", childs[j]);
                }

                else
                {
                    printf("ERROR while killing Child[%d]", childs[j]);
                }
            }

            exit(1);
        }

#ifdef WITH_SIGNALS /*To ensure that I send the SIGTERM Signal to the child process */
        if (global_flag == 1)
        {
            printf("Interruption of Child process creation!.\n");
            printf("\n");

            for (int j = 0; j <= i; j++) /*Killing all the created child processes*/
            {
                printf("Parent[%d]: sending SIGTERM signal.\n", pid);
                kill(childs[j], SIGTERM);
                //printf("Child[%d]: recived SIGTERM signal, terminating.\n",childs[j] );
            }

            break;
        }

#endif
    }

    printf("\n");
    printf("Parent[%d]: All Child Processes have been created.\n\n", pid);

    /* Wait for children to exit */
    int cont = 0;
    while (1)
    {
        int finishID = wait(NULL);
        if (finishID == -1)
        {
            break;
        }
        else
        {
            printf("Child[%d] has finished.\n", finishID);
            cont++;
        }
    }

    printf("\n");
    printf("Parent[%d]: %d Child Processes finished.\n", pid, cont);
    printf("\n");
    printf("Parent[%d]: Execution completed!\n", pid);

#ifdef WITH_SIGNALS
    for (int p = 0; p < _NSIG; p++)
    {
        signal(p, SIG_DFL); //3.e.parent.
    }

#endif
    exit(0);
}
