#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void signal_handler(int signal) {
    switch(signal) {
        case SIGCHLD:
            printf("received SIGCHLD\n");
            break;
	}
}

/*
 * create process tree:
 * \_ main
 *     \_ zombie-parent
 *         \_ zombie-1
 *         \_ zombie-2
 *         \_ zombie-3
 *         \_ zombie-4
 *         \_ zombie-5
 */
int main(int argc, char **argv) {
    int argv0size = strlen(argv[0]);

    // name main process
    memset(argv[0], 0, argv0size);
    snprintf(argv[0], argv0size, "%s", "main");

    if (signal(SIGCHLD, signal_handler) == SIG_ERR)
        printf("can't trap SIGCHLD\n");

    pid_t zombie_parent_pid = fork();
    if(zombie_parent_pid == 0) {
        // name zombie-parent process
        memset(argv[0], 0, argv0size);
        snprintf(argv[0], argv0size, "%s", "zombie-parent");
        for(int i = 1; i <= 5; ++i) {
            if(fork() == 0) {
                // name zombie process
                memset(argv[0], 0, argv0size);
                snprintf(argv[0], argv0size, "%s-%d", "zombie", i);
                printf("process zombie-%d will exit after %d seconds.\n", i, 10 + i * 5);
                sleep(10 + i * 5);
                printf("process zombie-%d exit.\n", i);
                exit(0);
            }
        }
        printf("zombie processes have been created, zombie-parent process will exit after 10 seconds.\n");
        sleep(10);
        exit(0);
    }

    waitpid(zombie_parent_pid, NULL, 0);
    printf("main process will exit after 300 seconds.\n");
    for(int i = 0; i < 300; ++i)
        sleep(1);
    printf("main process exit.\n");
    return 0;
}
