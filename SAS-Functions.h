#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct
{
    char *username;
    char *password;
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
    printf("please enter your username: ");
    new_user.username = (char *)malloc(25 * sizeof(char));
    if (new_user.username == NULL)
    {
        printf("memory allocation failed");
        exit(1);
    }
    fgets(new_user.username, 25, stdin);
    if (strlen(new_user.username) > 0 && new_user.username[strlen(new_user.username) - 1] == '\n')
    {
        new_user.username[strlen(new_user.username) - 1] = '\0';
    }
    printf("please enter your password: ");
    new_user.password = (char *)malloc(25 * sizeof(char));
    if (new_user.password == NULL)
    {
        printf("memory allocation failed");
        exit(1);
    }
    fgets(new_user.password, 25, stdin);
    if (strlen(new_user.password) > 0 && new_user.password[strlen(new_user.password) - 1] == '\n')
    {
        new_user.password[strlen(new_user.password) - 1] = '\0';
    }

    return new_user;
}

int Login(users user_array[], char *username, char *password, int count)
{

    if (strcmp(user_array[0].username, username) == 0 && strcmp(user_array[0].password, password) == 0)
    {
        return 2;

        for (int i = 1; i < count; i++)
        {
            if (strcmp(user_array[i].username, username) == 0 && strcmp(user_array[i].password, password) == 0)
            {
                return 1;
            }
        }

        return 0;
    }
}

tickets AddTicket(tickets ticket_array[], int ticket_count)
{
    tickets new_ticket;
    new_ticket.id = (int *)malloc(sizeof(int));
    *new_ticket.id = rand() % 100000000;
    printf("enter the reason of your complaint: ");
    new_ticket.reason = (char *)malloc(255 * sizeof(char));
    if (new_ticket.reason == NULL)
    {
        printf("memory allocation failed");
        exit(1);
    }
    fgets(new_ticket.reason, 255, stdin);
    if (strlen(new_ticket.reason) > 0 && new_ticket.reason[strlen(new_ticket.reason) - 1] == '\n')
    {
        new_ticket.reason[strlen(new_ticket.reason) - 1] = '\0';
    }
    printf("\nenter a detailed description of the problem you're facing: ");
    new_ticket.description = (char *)malloc(1024 * sizeof(char));
    if (new_ticket.description == NULL)
    {
        printf("memory allocation failed");
        exit(1);
    }
    fgets(new_ticket.description, 1024, stdin);
    if (strlen(new_ticket.description) > 0 && new_ticket.description[strlen(new_ticket.description) - 1] == '\n')
    {
        new_ticket.description[strlen(new_ticket.description) - 1] = '\0';
    }
    int category_choice = 0;
    printf("\n 1. Hardware \n 2. Software \n 3. Network \n 4. billing \n 5. Others \n");
    printf("enter the category of your complaint: ");
    new_ticket.category = (char *)malloc(255 * sizeof(char));
    scanf("%d", &category_choice);
    getchar();
    switch (category_choice)
    {
    case 1:
        new_ticket.category = "Hardware";
        break;
    case 2:
        new_ticket.category = "Software";
        break;
    case 3:
        new_ticket.category = "Network";
        break;
    case 4:
        new_ticket.category = "Billing";
        break;
    case 5:
        printf("enter your other category: ");
        new_ticket.category = (char *)malloc(255 * sizeof(char));
        fgets(new_ticket.category, 255, stdin);
        if (strlen(new_ticket.category) > 0 && new_ticket.category[strlen(new_ticket.category) - 1] == '\n')
        {
            new_ticket.category[strlen(new_ticket.category) - 1] = '\0';
        }
        break;
    default:
        printf("Invalid choice\n");
        break;
    }
    int status_choice;
    printf("\n1.ongoing\n2.resolved\n3.rejected\n enter the status of the issue: ");
    new_ticket.status = (char *)malloc(25 * sizeof(char));
    scanf("%d", &status_choice);
    getchar();
    switch (status_choice)
    {
    case 1:
        new_ticket.status = "Ongoing";
        break;
    case 2:
        new_ticket.status = "Resolved";
        break;
    case 3:
        new_ticket.status = "Rejected";
        break;
    default:
        printf("Invalid choice\n");
        break;
    }
    new_ticket.time = (char *)malloc(100 * sizeof(char));
    if (new_ticket.time == NULL)
    {
        printf("memory allocation failed");
        exit(1);
    }
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    strftime(new_ticket.time, 100, "%d-%m-%Y %H:%M:%S", &tm);
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