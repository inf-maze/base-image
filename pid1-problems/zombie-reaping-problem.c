#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdarg.h>
#include <time.h>

void write_log(const char *format, ...) {
    time_t timer;
    char buffer[26];
    struct tm* tm_info;

    time(&timer);
    tm_info = localtime(&timer);

    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);

    printf("%s ", buffer);
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\n");
}

void signal_handler(int signal) {
    switch(signal) {
        case SIGCHLD:
            write_log("Got SIGCHLD.");
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
        write_log("can't trap SIGCHLD.");

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
                write_log("process zombie-%d will exit after %d seconds.", i, 10 + i * 5);
                sleep(10 + i * 5);
                write_log("process zombie-%d exit.", i);
                exit(0);
            }
        }
        write_log("zombie processes have been created, zombie-parent process will exit after 10 seconds.");
        sleep(10);
        exit(0);
    }

    waitpid(zombie_parent_pid, NULL, 0);
    write_log("main process will exit after 60 seconds.");
    for(int i = 0; i < 60; ++i) sleep(1);
    write_log("main process exit.");
    return 0;
}
