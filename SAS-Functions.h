#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#define USERNAME_SIZE 25
#define PASSWORD_SIZE 25
#define REASON_SIZE 255
#define DESCRIPTION_SIZE 1024
#define CATEGORY_SIZE 255
#define STATUS_SIZE 25
#define TIME_SIZE 100
#define MAX_TICKETS 100
#define MAX_USERS 100

typedef struct
{
    char *username;
    char *password;
    char *role;
} users;

typedef struct
{
    int *id;
    char *reason;
    char *description;
    char *category;
    char *status;
    char *time;
} tickets;

users SignUp(users user_array[])
{
    users new_user;
    int valid_password = 0;
    printf("please enter your username: ");
    new_user.username = (char *)malloc(USERNAME_SIZE * sizeof(char));
    fgets(new_user.username, USERNAME_SIZE, stdin);
    new_user.username[strcspn(new_user.username, "\n")] = '\0';

    do
    {
        printf("please enter your password: ");
        new_user.password = (char *)malloc(PASSWORD_SIZE * sizeof(char));
        fgets(new_user.password, PASSWORD_SIZE, stdin);
        new_user.password[strcspn(new_user.password, "\n")] = '\0';

        int has_uppercase = 0;
        int has_lowercase = 0;
        int has_digit = 0;
        int has_space = 0;
        int has_special = 0;
        for (int i = 0; i < strlen(new_user.password); i++)
        {
            if (isupper(new_user.password[i]))
            {
                has_uppercase = 1;
            }
            else if (islower(new_user.password[i]))
            {
                has_lowercase = 1;
            }
            else if (isdigit(new_user.password[i]))
            {
                has_digit = 1;
            }
            else if (isspace(new_user.password[i]))
            {
                has_space = 1;
            }
            else if (ispunct(new_user.password[i]))
            {
                has_special = 1;
            }
        }
        valid_password = 1;

        if (!has_lowercase)
        {
            printf("Password must contain at least one lowercase letter.\n");
            valid_password = 0;
        }

        if (!has_uppercase)
        {
            printf("Password must contain at least one uppercase letter.\n");
            valid_password = 0;
        }
        if (!has_digit)
        {
            printf("Password must contain at least one digit.\n");
            valid_password = 0;
        }

        if (has_space)
        {
            printf("Password must not contain spaces.\n");
            valid_password = 0;
        }

        if (!has_special)
        {
            printf("Password must contain at least one special character.\n");
            valid_password = 0;
        }

        if (strlen(new_user.password) < 8)
        {
            printf("Password must be at least 8 characters long.\n");
            valid_password = 0;
        }

        if (valid_password == 0)
        {
            free(new_user.password);
        }
    } while (!valid_password);

    if (user_array[0].username == NULL)
    {
        user_array[0] = new_user;
    }
    else
    {
        for (int i = 1; i < MAX_USERS; i++)
        {
            if (user_array[i].username == NULL || user_array[i].username[0] == '\0')
            {
                user_array[i] = new_user;
                break;
            }
        }
    }

    printf("Sign up successful\n");

    return new_user;
}

int Login(users user_array[], char *username, char *password, int count)
{
    if (strcmp(user_array[0].username, username) == 0 && strcmp(user_array[0].password, password) == 0)
    {
        return 2;
    }
    for (int i = 1; i < count; i++)
    {
        if (strcmp(user_array[i].username, username) == 0 && strcmp(user_array[i].password, password) == 0)
        {
            return 1;
        }
    }
    return 0;
}

tickets AddTicket(tickets ticket_array[], int ticket_count)
{
    tickets new_ticket;
    new_ticket.id = (int *)malloc(sizeof(int));
    *new_ticket.id = rand() % 100000000;

    printf("enter the reason of your complaint: ");
    new_ticket.reason = (char *)malloc(REASON_SIZE * sizeof(char));
    fgets(new_ticket.reason, REASON_SIZE, stdin);
    new_ticket.reason[strcspn(new_ticket.reason, "\n")] = '\0';

    printf("\nenter a detailed description of the problem you're facing: ");
    new_ticket.description = (char *)malloc(DESCRIPTION_SIZE * sizeof(char));
    fgets(new_ticket.description, DESCRIPTION_SIZE, stdin);
    new_ticket.description[strcspn(new_ticket.description, "\n")] = '\0';

    int category_choice = 0;
    printf("\n 1. Hardware \n 2. Software \n 3. Network \n 4. Billing \n 5. Others \n");
    printf("enter the category of your complaint: ");
    scanf("%d", &category_choice);
    getchar();

    switch (category_choice)
    {
    case 1:
        new_ticket.category = strdup("Hardware");
        break;
    case 2:
        new_ticket.category = strdup("Software");
        break;
    case 3:
        new_ticket.category = strdup("Network");
        break;
    case 4:
        new_ticket.category = strdup("Billing");
        break;
    case 5:
        new_ticket.category = (char *)malloc(CATEGORY_SIZE * sizeof(char));
        printf("enter your other category: ");
        fgets(new_ticket.category, CATEGORY_SIZE, stdin);
        new_ticket.category[strcspn(new_ticket.category, "\n")] = '\0';
        break;
    default:
        printf("Invalid choice\n");
        break;
    }

    int status_choice;
    printf("\n1. Ongoing\n2. Resolved\n3. Rejected\n enter the status of the issue: ");
    new_ticket.status = (char *)malloc(STATUS_SIZE * sizeof(char));
    scanf("%d", &status_choice);
    getchar();

    switch (status_choice)
    {
    case 1:
        new_ticket.status = strdup("Ongoing");
        break;
    case 2:
        new_ticket.status = strdup("Resolved");
        break;
    case 3:
        new_ticket.status = strdup("Rejected");
        break;
    default:
        printf("Invalid choice\n");
        break;
    }

    new_ticket.time = (char *)malloc(TIME_SIZE * sizeof(char));
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    strftime(new_ticket.time, TIME_SIZE, "%d-%m-%Y %H:%M:%S", &tm);

    return new_ticket;
}

void DisplayTickets(tickets ticket_array[], int ticket_count)
{
    for (int i = 0; i < ticket_count; i++)
    {
        printf("id: %d\n", *ticket_array[i].id);
        printf("reason: %s\n", ticket_array[i].reason);
        printf("description: %s\n", ticket_array[i].description);
        printf("category: %s\n", ticket_array[i].category);
        printf("status: %s\n", ticket_array[i].status);
        printf("time: %s\n", ticket_array[i].time);
        printf("\n");
    }
}

void FreeUsers(users user_array[], int count)
{
    for (int i = 0; i < count; i++)
    {
        free(user_array[i].username);
        free(user_array[i].password);
    }
}

void FreeTickets(tickets ticket_array[], int ticket_count)
{
    for (int i = 0; i < ticket_count; i++)
    {
        free(ticket_array[i].id);
        free(ticket_array[i].reason);
        free(ticket_array[i].description);
        free(ticket_array[i].category);
        free(ticket_array[i].status);
        free(ticket_array[i].time);
    }
}
