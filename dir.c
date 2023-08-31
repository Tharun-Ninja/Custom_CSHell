#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

// funtion to remove directory recursively
int rmMydir(char *path)
{
    // dirent is a struct
    struct dirent *elements;
    DIR *dir;

    // to Open that directory
    dir = opendir(path);

    // loop through all the elements in that directory
    while ((elements = readdir(dir)) != NULL)
    {
        // we don not want to delete "." and ".."
        if (strcmp(elements->d_name, ".") != 0 && strcmp(elements->d_name, "..") != 0)
        {
            // Changing path to the to include that elements
            char current_path[1024] = "";
            strcat(current_path, path);
            strcat(current_path, "/");
            strcat(current_path, elements->d_name);

            // printf("cur: %s\n", current_path);

            // Recursively deleting if a element is found to be a directory
            if (elements->d_type == DT_DIR)
                rmMydir(current_path);
            else
                remove(current_path);
        }
    }
    closedir(dir);
    return rmdir(path);
}

int main(int argc, char *argv[])
{
    // saving the current path
    char current_dir[1024];
    getcwd(current_dir, sizeof(current_dir));
    // printf("in dir: %s\n", current_dir);

    if (argc == 2 || argc > 4)
    {
        printf("Invalid command\n");
        printf("Usage: dir {-r|-v} <Directory>\n");
        exit(1);
    }
    // checking for options
    if (argv[2][0] == '-')
    {
        // checking for -r
        if (strcmp(argv[2], "-r") == 0)
        {
            char *folder = argv[3];
            strcat(argv[0], "/");
            strcat(argv[0], folder);

            // if dir exirsts then deleting and recreating
            if (mkdir(argv[0], S_IRWXU) != 0)
            {
                if (rmMydir(argv[0]) == 0)
                {
                    if (mkdir(argv[0], S_IRWXU) == -1)
                    {
                        perror("Error: ");
                        exit(1);
                    }
                }
                else
                {
                    perror("Error: ");
                    exit(1);
                }
            }
            // printf("current Dir: %s\n", argv[0]);s
        }
        // checking if -v
        else if (strcmp(argv[2], "-v") == 0)
        {
            char *folder = argv[3];
            printf("%s Created in : %s\n", folder, argv[0]);
            strcat(argv[0], "/");
            strcat(argv[0], folder);

            // Creating directory
            if (mkdir(argv[0], S_IRWXU) != 0)
            {
                perror("Error");
                exit(1);
            }
            // printing steps since -v
            printf("Directory changed to: %s\n", argv[0]);
        }
        else
        {
            printf("invalid option\n");
            printf("Usage: dir {-r|-v} <Directory>\n");
            exit(1);
        }
    }
    else
    {

        if (argc != 3)
        {
            printf("Invalid command\n");
            printf("Usage: dir {-r|-v} <Directory>\n");
            exit(1);
        }

        // Concatinating new folder name with path
        char *folder = argv[2];
        strcat(argv[0], "/");
        strcat(argv[0], folder);

        // creating directory
        if (mkdir(argv[0], S_IRWXU) != 0)
        {
            perror("Error");
            exit(1);
        }
    }
}