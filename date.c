#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc == 2 || argc > 5)
    {
        printf("Invalid command\n");
        printf("Usage: date {-R|-d} <filename>\n");
        exit(1);
    }
    // checking for options
    if (argv[2][0] == '-')
    {
        // checking for -R
        if (strcmp(argv[2], "-R") == 0)
        {
            char *filename;
            if (argc == 4)
                filename = argv[3];
            else
            {
                printf("Invalid command\n");
                printf("Usage: date {-R|-d} <filename>\n");
                exit(1);
            }

            struct stat fileTime;
            if (stat(filename, &fileTime) == -1)
            {
                perror("Error");
                return 1;
            }

            char rfcTime[64];
            strftime(rfcTime, sizeof(rfcTime), "%a, %d %b %Y %H:%M:%S %z", gmtime(&fileTime.st_mtime));

            // printing time
            printf("%s\n", rfcTime);
        }
        // checking for -d
        else if (strcmp(argv[2], "-d") == 0)
        {
            char *filename;
            if (argc == 5)
                filename = argv[4];
            else
            {
                printf("Invalid command\n");
                printf("Usage: date {-R|-d} <filename>\n");
                exit(1);
            }

            // getting the file stats
            struct stat fileTime;
            if (stat(filename, &fileTime) == -1)
            {
                perror("Error");
                return 1;
            }

            // to remove the single quotes '' from the argument
            int length = strlen(argv[3]);
            if (argv[3][0] == '\'' && argv[3][length - 1] == '\'')
            {
                memmove(argv[3], argv[3] + 1, length - 2);
                argv[3][length - 2] = '\0';
            }
            else
            {
                printf("String not supported\n");
                printf("supported STRINGS: 'tomorrow', 'yesterday'\n");
                printf("Usage: date {-R|-d} <filename>\n");
                exit(1);
            }

            // Checking is STRING is 'yesterday'
            if (strcmp(argv[3], "yesterday") == 0)
            {

                time_t current_time = fileTime.st_mtime;
                // struct for saving time
                struct tm *modification_tm = localtime(&current_time);
                modification_tm->tm_mday--;
                time_t yesterday_time = mktime(modification_tm);

                printf("%s", ctime(&yesterday_time));
            }
            // checking for tomorrow
            else if (strcmp(argv[3], "tomorrow") == 0)
            {
                time_t current_time = fileTime.st_mtime;
                struct tm *modification_tm = localtime(&current_time);
                modification_tm->tm_mday++;
                time_t tomorrow_time = mktime(modification_tm);

                printf("%s", ctime(&tomorrow_time));
            }
            else
            {
                printf("String not supported\n");
                printf("supported STRINGS: 'tomorrow', 'yesterday'\n");
                printf("Usage: date {-R|-d} <filename>\n");
                exit(1);
            }
        }
        else
        {
            printf("Invalid Command\n");
            printf("Usage: date {-R|-d} <filename>\n");
            exit(1);
        }
    }
    // if no arguments
    else
    {
        char *filename;
        if (argc == 3)
            filename = argv[2];
        else
        {
            printf("Invalid Command\n");
            printf("Usage: date {-R|-d} <filename>\n");
            exit(1);
        }

        struct stat fileTime;
        if (stat(filename, &fileTime) == -1)
        {
            perror("Error getting file status");
            return 1;
        }

        // printing files latest modified date
        printf("%s", ctime(&fileTime.st_mtime));
    }
}
