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
    char admin_username[25] = "admin";
    for (int i = 0; i < count; i++)
    {
        if (strcmp(user_array[i].username, admin_username) == 0 && strcmp(user_array[i].password, password) == 0)
        {
            return 2;
        }
        else if (strcmp(user_array[i].username, username) == 0 && strcmp(user_array[i].password, password) == 0)
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

int main()
{
    int initial_choice = 0;
    int user_choice = 0;
    int admin_choice = 0;
    users user_array[100];
    tickets ticket_array[100];
    int count = 0;
    int ticket_count = 0;
    char username[25];
    char password[25];
    int login_status = 0;

    do
    {
        printf("1. Sign Up\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("enter your choice: ");
        scanf("%d", &initial_choice);
        getchar();
        switch (initial_choice)
        {
        case 1:
            user_array[count] = SignUp(user_array);
            printf("Sign up successful\n");
            count++;
            break;

        case 2:
            printf("enter your username: ");
            fgets(username, 25, stdin);
            if (strlen(username) > 0 && username[strlen(username) - 1] == '\n')
            {
                username[strlen(username) - 1] = '\0';
            }
            printf("enter your password: ");
            fgets(password, 25, stdin);
            if (strlen(password) > 0 && password[strlen(password) - 1] == '\n')
            {
                password[strlen(password) - 1] = '\0';
            }

            login_status = Login(user_array, username, password, count);

            switch (login_status)
            {
            case 1:
                printf("Login successful\n");
                printf("Welcome %s\n", username);
                printf("1. Add ticket\n");
                printf("2. Exit\n");
                printf("Enter your choice: ");
                scanf("%d", &user_choice);
                getchar();
                switch (user_choice)
                {
                case 1:
                    ticket_array[ticket_count] = AddTicket(ticket_array, ticket_count);
                    ticket_count++;
                    break;
                case 2:
                    break;
                default:
                    printf("Invalid choice\n");
                    break;
                }
                free(login_status);
                break;
            case 2:
            {
                printf("Admin login successful\n");
                printf("Welcome to the admin panel\n");
                printf("1. Display tickets\n");
                printf("2. Exit\n");
                printf("Enter your choice: ");
                scanf("%d", &user_choice);
                getchar();
                switch (admin_choice)
                {
                case 1:
                    if (ticket_count == 0)
                    {
                        printf("No tickets found\n");
                    }
                    else
                    {
                        printf("Displaying tickets\n");
                        printf("there are %d tickets\n", ticket_count);
                        DisplayTickets(ticket_array, ticket_count);
                    }
                    break;
                case 2:
                    break;
                default:
                    printf("Invalid choice\n");
                    break;
                }
            }
            }
        }
    } while (initial_choice != 3)

        ;
    for (int i = 0; i < count; i++)
    {
        free(user_array[i].username);
        free(user_array[i].password);
    }
    for (int i = 0; i < ticket_count; i++)
    {
        free(ticket_array[i].id);
        free(ticket_array[i].reason);
        free(ticket_array[i].description);
        free(ticket_array[i].category);
        free(ticket_array[i].status);
        free(ticket_array[i].time);
    }
    return 0;
}
