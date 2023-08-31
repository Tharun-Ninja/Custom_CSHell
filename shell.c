#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int wordCount(int option, char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error Opening file: %s\n", filename);
        printf("Usage: word {-n/d} <filenames>\n");
        return -1;
    }
    int isWord = 0;
    int wordCounter = 0;
    char ch;

    while ((ch = fgetc(file)) != EOF)
    {
        if ((option != -1) && (option != 1))
        {
            // consider \n not a word
            if (ch == ' ' || ch == '\t' || ch == '\n')
                isWord = 0;
            else if (isWord == 0)
            {
                wordCounter++;
                isWord = 1;
            }
        }
        else
        {
            // consider \n as a word
            if (ch == ' ' || ch == '\t')
                isWord = 0;
            else if (isWord == 0)
            {
                wordCounter++;
                isWord = 1;
            }
        }
    }

    fclose(file);

    return wordCounter;
}

int main()
{
    // Saves the current Directory
    char current_dir[1024];
    getcwd(current_dir, sizeof(current_dir));

    while (1)
    {
        char command[100];
        printf("\033[0;36m(ninja)🥷 %s|> \033[0;37m", current_dir);
        scanf(" %[^\n]s", command);

        char *token = strtok(command, " ");
        int count = 1;
        char *argv[6];

        // argv[0] is always the current path
        argv[0] = current_dir;

        // Making an args array to pass to the exec while calling a command
        while (token != NULL)
        {
            argv[count++] = token;
            token = strtok(NULL, " ");
        }
        argv[count] = NULL;
        int argc = count;

        // checking if user called dir command
        if (strcmp(argv[1], "dir") == 0)
        {
            argv[1] = "./dir";
            int status = 9898;
            // Creating fork
            int rc = fork();
            // child process
            if (rc == 0)
            {
                // using execvp to call the dir command
                if (execvp(argv[1], argv) == -1)
                {
                    perror("execvp");
                    exit(-1);
                }
                else
                {
                    exit(1);
                }
            }
            // Parent process
            else if (rc > 0)
            {
                wait(&status);

                // status is 0 is child executed correctly
                if (status == 0)
                {
                    // changing dir in parent process
                    if (argc == 4)
                    {
                        char *directory = argv[3];
                        strcat(current_dir, "/");
                        strcat(current_dir, directory);
                    }
                    else
                    {
                        char *directory = argv[2];
                        strcat(current_dir, "/");
                        strcat(current_dir, directory);
                    }
                }
            }
            else
            {
                perror("fork");
            }
        }
        // checking if user called date command
        else if (strcmp(argv[1], "date") == 0)
        {
            argv[1] = "./date";
            // creating fork
            int rc = fork();
            if (rc == 0)
            {
                // using execvp to call the date command in child process
                if (execvp(argv[1], argv) == -1)
                {
                    perror("execvp");
                    exit(1);
                }
            }
            // Parent process
            else if (rc > 0)
            {
                wait(NULL);
            }
            else
            {
                perror("fork");
            }
        }

        // checking if user called word command
        // word is an internal command
        else if (strcmp(argv[1], "word") == 0)
        {
            if ((argc == 2) || (argc > 5))
            {
                printf("Usage: word {-n/d} <filenames>\n");
                continue;
            }
            // checking if they gave an option since it starts with -
            if (argv[2][0] == '-')
            {
                // checking for -n option
                if (strcmp(argv[2], "-n") == 0)
                {
                    if (argc != 4)
                    {
                        printf("Usage: word {-n/d} <filenames>\n");
                        continue;
                    }

                    char *filename = argv[3];
                    // Doing wordCount
                    int wc = wordCount(0, filename);
                    if (wc == -1)
                        continue;
                    printf("Count: %d\n", wc);
                }
                // checking for -d option
                else if (strcmp(argv[2], "-d") == 0)
                {
                    if (argc != 5)
                    {
                        printf("Usage: word {-n/d} <filename1> <filename2>\n");
                        continue;
                    }
                    char *filename1 = argv[3];
                    char *filename2 = argv[4];

                    int wc1 = wordCount(1, filename1);
                    int wc2 = wordCount(1, filename2);

                    if (wc1 == -1 || wc2 == -1)
                        continue;

                    printf("Count of file %s: %d\n", filename1, wc1);
                    printf("Count of file %s: %d\n", filename1, wc2);
                    printf("Difference Count: %d\n", wc1 - wc2);
                }
                else
                {
                    printf("Invalid Option\n");
                    printf("Usage: word {-n/d} <filenames>\n");

                    continue;
                }
            }
            else
            {
                if (argc != 3)
                {

                    printf("Invalid Option\n");
                    printf("Usage: word {-n/d} <filenames>\n");

                    continue;
                }
                char *filename = argv[2];
                int wc = wordCount(-1, filename);
                if (wc == -1)
                    continue;
                printf("Count: %d\n", wc);
            }
        }
        // if no commands matched then command not found
        else
        {
            printf("Command not Found\n");
        }
    }
}