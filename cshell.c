/**
 * Author: Jagdeep Singh
 * Date: JUne 9 2023
 * File: This file implements a simple shell with few built in commands
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>

// variables that can be changed as per the user input
#define MAX_COMMAND_SIZE 150
#define MAX_ARGS 40
#define MAX_HISTORY 100
#define MAX_COMMAND_LENGTH 100

// struct for the environment variables
typedef struct
{
    char name[MAX_COMMAND_LENGTH];
    char value[MAX_COMMAND_LENGTH];
} EnvVariable;

int logcount = 0;

// Arrays to store commands, exitcodes and time
char loghistory[MAX_HISTORY][MAX_COMMAND_LENGTH];
char timeHistory[MAX_HISTORY][30];
int exitCodes[MAX_HISTORY];
EnvVariable envVariables[MAX_ARGS];

// Global variables
int envCount = 0;
char *color = "\033[0;37m";

// Method to print the arguments
void printArgument(char *argument)
{
    printf("%s ", argument);
}

// Method to check if string contain only spaces
bool isOnlySpaces(char *str)
{
    while (*str != '\0')
    {
        if (!isspace((unsigned char)*str))
        {
            return false;
        }
        str++;
    }
    return true;
}

// Method  to make string case insensitive
int caseInsensitiveStringCompare(const char *str1, const char *str2)
{
    while (*str1 && *str2)
    {
        if (tolower((unsigned char)*str1) != tolower((unsigned char)*str2))
        {
            return (tolower((unsigned char)*str1) - tolower((unsigned char)*str2));
        }
        str1++;
        str2++;
    }

    return tolower((unsigned char)*str1) - tolower((unsigned char)*str2);
}

// Method to check if the variable name is valid
bool isValidVariableName(char *variableName)
{
    for (int i = 0; i < strlen(variableName); i++)
    {
        if (!isalnum(variableName[i]) && variableName[i] != '_' && variableName[i] != '$')
        {
            return false;
        }
    }
    return true;
}

// Method to print the log history
void printhistory()
{
    for (int i = 0; i < logcount; i++)
    {
        printf("%s\n %s %d\n", timeHistory[i], loghistory[i], exitCodes[i]);
    }
}

// Mewthod that changes the theme of the shell according to the input provided
void changeTheme(char *args)
{
    // check if the coclor name is stored in a variable
    for (int k = 0; k < envCount; k++)
    {
        if (caseInsensitiveStringCompare(args, envVariables[k].name) == 0)
        {
            args = envVariables[k].value;
            break;
        }
    }

    if (strcmp(args, "red") == 0)
    {
        color = "\033[0;31m";
        printf("%s", color); // Set text color to red
        exitCodes[logcount - 1] = 0;
    }
    else if (strcmp(args, "blue") == 0)
    {
        color = "\033[0;34m";
        printf("%s", color); // Set text color to blue
        exitCodes[logcount - 1] = 0;
    }
    else if (strcmp(args, "green") == 0)
    {
        color = "\033[0;32m";
        printf("%s", color); // Set text color to green
        exitCodes[logcount - 1] = 0;
    }
    else
    {
        printf("Unsupported theme.\n");
        exitCodes[logcount - 1] = -1;
    }
}

// Function that parses the entered command and calls the appropriate method
void parseCommand(char *command, char *timee)
{
    // block to check if the entered command is to assign a variable

    // block to check if there are more than one asssignment operator in the variable asssignment
    if (command[0] == '$')
    {
        // Storing the variable assignment in log history
        strcpy(loghistory[logcount], command);
        strcpy(timeHistory[logcount], timee); // Store the time string
        logcount++;

        int equalCount = 0;
        for (int i = 0; command[i] != '\0'; i++)
        {
            if (command[i] == '=')
            {
                equalCount++;
            }
        }
        if (equalCount > 1)
        {
            printf("Error: Invalid argument with multiple '=' signs.\n");
            exitCodes[logcount - 1] = -1;
        }
        else
        {

            // Split the command into variable name and value
            char *token = strtok(command, "=");
            char *variable = token;

            if (!isValidVariableName(variable))
            {
                printf("Not a valid Variable Name\n");
                exitCodes[logcount - 1] = -1;
                return;
            }

            token = strtok(NULL, "=");
            char *value = token;

            if ((value == NULL || value[0] == ' ') || variable[strlen(variable) - 1] == ' ' || (strchr(variable, '/') != NULL))
            {
                printf("Variable value expected\n");
                exitCodes[logcount - 1] = -1;
            }
            else
            {
                // THis block checks if the variable already exsits or is a newly created variable
                int found = 0;
                for (int i = 0; i < envCount; i++)
                {
                    if (caseInsensitiveStringCompare(envVariables[i].name, variable) == 0)
                    {
                        strcpy(envVariables[i].value, value);
                        found = 1;
                        break;
                    }
                }
                if (!found)
                {
                    if (envCount < MAX_ARGS)
                    {
                        strcpy(envVariables[envCount].name, variable);
                        strcpy(envVariables[envCount].value, value);
                        envCount++;
                    }
                    else
                    {
                        printf("Maximum number of environment variables reached.\n");
                    }
                }
            }
        }
    }
    else
    {
        // Parse the command into program name and arguments
        char *args[MAX_ARGS];
        char *token = strtok(command, " ");
        // Store the command in the history
        strcpy(loghistory[logcount], token);
        strcpy(timeHistory[logcount], timee); // Store the time string
        logcount++;

        int i = 0;
        while (token != NULL && i < MAX_ARGS - 1)
        {
            args[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        args[i] = NULL;

        // Check if it's a built-in command
        if (strcmp(args[0], "exit") == 0)
        {
            if (args[1] != NULL)
            {
                printf("Error: Too many arguments\n");
                exitCodes[logcount - 1] = -1;
                return;
            }
            printf("Bye!\n");
            printf("\033[0;37m");
            exit(0);
        }
        // Code block fro print command
        else if (strcmp(args[0], "print") == 0)
        {
            if (args[1] == NULL)
            {
                printf("Missing keyword or command, or permission problem.\n");
                exitCodes[logcount - 1] = -1;
                return;
            }
            else
            {
                // for loop to check if command contains more than one env variables and if  are  they  all present
                for (int i = 1; args[i] != NULL; i++)
                {
                    char *variable = args[i];
                    if (variable[0] == '$')
                    {
                        int found = 0;
                        for (int i = 0; i < envCount; i++)
                        {
                            if (caseInsensitiveStringCompare(envVariables[i].name, variable) == 0)
                            {
                                found = 1;
                                break;
                            }
                        }
                        if (!found)
                        {
                            printf("Error: No Environment variable %s found\n", variable);
                            exitCodes[logcount - 1] = -1;
                            return;
                        }
                    }
                }
                // If the variables are all present print them
                for (int i = 1; args[i] != NULL; i++)
                {
                    char *variable = args[i];
                    if (variable[0] == '$')
                    {
                        for (int i = 0; i < envCount; i++)
                        {
                            if (caseInsensitiveStringCompare(envVariables[i].name, variable) == 0)
                            {
                                printf("%s ", envVariables[i].value);

                                break;
                            }
                        }
                    }
                    else
                    {
                        printArgument(args[i]);
                    }
                }
            }
            printf("\n");
            exitCodes[logcount - 1] = 0;
        }

        // code block for the log command
        else if (strcmp(args[0], "log") == 0)
        {
            if (args[1] != NULL)
            {
                printf("Error: Too many arguments\n");
                exitCodes[logcount - 1] = -1;
                return;
            }
            printhistory();
            exitCodes[logcount - 1] = 0;
        }
        // code block for theme command
        else if (strcmp(args[0], "theme") == 0)
        {
            if (args[1] == NULL)
            {
                printf("Error: Not a valid command\n");
                exitCodes[logcount - 1] = -1;
                return;
            }
            if (args[2] != NULL)
            {
                printf("Error: Too many arguments\n");
                exitCodes[logcount - 1] = -1;
                return;
            }
            changeTheme(args[1]);
        }
        else
        {
            // Create a pipe
            int pipefd[2];
            pipe(pipefd);

            pid_t pid = fork();
            if (pid == 0)
            {
                // Child process

                // Redirect standard output to the write end of the pipe
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);

                // Execute the command

                for (int j = 0; args[j] != NULL; j++)
                {
                    for (int k = 0; k < envCount; k++)
                    {
                        if (caseInsensitiveStringCompare(args[j], envVariables[k].name) == 0)
                        {
                            args[j] = envVariables[k].value;
                            break;
                        }
                    }
                }
                execvp(args[0], args);

                printf("%s", color);
                // If execvp returns, it means the command failed
                printf("Missing keyword or command, or permission problem\n");
                exit(1);
            }
            else if (pid > 0)
            {
                // Parent process

                // Wait for the child process to finish
                int status;
                waitpid(pid, &status, 0);

                // Read the output from the read end of the pipe
                close(pipefd[1]);
                char output[MAX_COMMAND_SIZE];
                read(pipefd[0], output, MAX_COMMAND_SIZE);
                close(pipefd[0]);

                // Print the output
                printf("%s", output);

                // Store the exit code
                exitCodes[logcount - 1] = -(WEXITSTATUS(status));
            }
            else
            {
                // Fork failed
                printf("Error: Fork failed\n");
                exitCodes[logcount - 1] = -1;
            }
        }
    }
}

// Function to  check if the command is null and make the time objects
void executeCommand(char *command)
{
    if (command == NULL || strlen(command) == 0 || isOnlySpaces(command))
    {
        printf("Error: Enter a valid command.\n");
        return;
    }
    else
    {
        // Get the current time
        time_t rawTime;
        time(&rawTime);
        struct tm *timeInfo = localtime(&rawTime);
        char timee[30];
        strftime(timee, sizeof(timee), "%a %b %d %H:%M:%S %Y", timeInfo);

        parseCommand(command, timee);
    }
}

// Main function to run the program in either script mode or interactive mode
int main(int argc, char *argv[])
{
    char command[MAX_COMMAND_SIZE];

    FILE *file = NULL;

    if (argc > 1)
    {
        file = fopen(argv[1], "r");
        if (file == NULL)
        {
            printf("Unable to read script file: %s\n", argv[1]);
            return 1;
        }

        FILE *file1 = fopen(argv[1], "a");
        if (file1 == NULL)
        {
            printf("Could not open file for writing.\n");
            return 1;
        }

        fputc('\n', file1);

        fclose(file1);

        while (1)
        {

            if (file != NULL && fgets(command, sizeof(command), file) != NULL)
            {
                // Remove the newline character
                command[strcspn(command, "\n")] = '\0';
                printf("\033[0;37m"); // Set text color to white for user input
            }

            else
            {
                break;
            }

            printf("%s", color);
            executeCommand(command);
        }

        if (file != NULL)
        {
            fclose(file);
        }
    }
    else
    {
        while (1)
        {
            printf("cshell$ ");

            printf("\033[0;37m"); // Set text color to white for user input
            fgets(command, sizeof(command), stdin);

            // Remove the newline character
            command[strcspn(command, "\n")] = '\0';

            printf("%s", color);
            executeCommand(command);
        }
    }
    return 0;
}
