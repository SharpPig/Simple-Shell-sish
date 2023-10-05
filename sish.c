#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 64
#define MAX_LINE 1024
#define MAX_HISTORY 10

char history[MAX_HISTORY][MAX_LINE];
int history_count = 0;

int execute_cd(char* args[]) {
    if (args[1] == NULL) {
        // If the user did not provide a directory argument, return an error
        fprintf(stderr, "sish: cd: no directory specified\n");
        return 1;
    } else {
        if (chdir(args[1]) != 0) {
            perror("sish");
            return 1;
        }
    }
    return 0;
}


void execute_command(char *args[]) {
    if (args[0] == NULL) {
        return; // empty command, do nothing
    } else if (strcmp(args[0], "cd") == 0) {
        // change directory
        if (args[1] == NULL) {
            // no argument, change to home directory
            chdir(getenv("HOME"));
        } else {
            // change to specified directory
            if (chdir(args[1]) < 0) {
                perror("cd");
            }
        }
    } else if (strcmp(args[0], "history") == 0) {
        // print command history
        int i;
        if (args[1] != NULL && strcmp(args[1], "-c") == 0) {
            // clear history
            history_count = 0;
        } else if (args[1] != NULL) {
            // execute command from history
            int offset = atoi(args[1]);
            if (offset >= 0 && offset < history_count) {
                char *command = history[offset];
                printf("Executing command #%d: %s", offset, command);
                // tokenize command and execute it
                int arg_count = 0;
                char *token = strtok(command, " \t\n"); // whitespace as delimiter
                while (token != NULL) {
                    if (arg_count >= MAX_ARGS - 1) {
                        fprintf(stderr, "Too many arguments\n");
                        arg_count = -1;
                        break;
                    }
                    args[arg_count++] = token;
                    token = strtok(NULL, " \t\n");
                }
                if (arg_count == -1) {
                    return; // restart loop to read next line of input
                }
                args[arg_count] = NULL; // terminate argument list
                execute_command(args);
            } else {
                printf("Invalid history offset: %s\n", args[1]);
            }
        } else {
            // print command history
            for (i = 0; i < history_count; i++) {
                printf("%d %s", i, history[i]);
            }
        }
    } else if (strcmp(args[0], "exit") == 0) {
        exit(0); // exit shell
    } else {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            // child process, execute command
            if (execvp(args[0], args) < 0) {
                perror("execvp");
                exit(1);
            }
        } else {
            // parent process, wait for child to complete
            waitpid(pid, NULL, 0);
        }
    }
}



int main() {
    char line[MAX_LINE];
    char *args[MAX_ARGS];

    while (1) {
        // print shell prompt
        printf("sish> ");
        fflush(stdout);

        // read line of input
        if (fgets(line, MAX_LINE, stdin) == NULL) {
            // handle EOF or error while reading input
            if (feof(stdin)) {
                printf("\n"); // print newline to match shell behavior
                exit(0);
            } else {
                perror("read");
                exit(1);
            }
        }

        // add command to history
        if (history_count < MAX_HISTORY) {
            strcpy(history[history_count], line);
            history_count++;
        } else {
            // shift history array to make space for new command
            int i;
            for (i = 1; i < MAX_HISTORY; i++) {
                strcpy(history[i-1], history[i]);
            }
            strcpy(history[MAX_HISTORY-1], line);
        }

        // tokenize line into arguments
        int arg_count = 0;
        char *token = strtok(line, " \t\n"); // whitespace as delimiter
        while (token != NULL) {
            if (arg_count >= MAX_ARGS - 1) {
                fprintf(stderr, "Too many arguments\n");
                arg_count = -1;
                break;
            }
            args[arg_count++] = token;
            token = strtok(NULL, " \t\n");
        }
        if (arg_count == -1) {
            continue; // restart loop to read next line of input
        }
        args[arg_count] = NULL; // terminate argument list

        // execute command
        execute_command(args);
    }

    return 0;
}
