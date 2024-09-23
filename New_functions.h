#ifndef TICKET_MANAGEMENT_H
#define TICKET_MANAGEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

#define USERNAME_SIZE 25
#define PASSWORD_SIZE 25
#define REASON_SIZE 255
#define DESCRIPTION_SIZE 1024
#define CATEGORY_SIZE 255
#define PRIORITY_SIZE 25
#define CLIENT_SIZE 40
#define STATUS_SIZE 25
#define TIME_SIZE 100
#define MAX_TICKETS 100
#define MAX_USERS 100
#define MAX_ROLE_SIZE 25

typedef enum
{
    ADMIN,
    AGENT,
    USER
} Role;

typedef enum
{
    PENDING,
    IN_PROGRESS,
    RESOLVED,
    CLOSED
} Status;

typedef enum
{
    DEFECTIVE_PRODUCT,
    CUSTOMER_SERVICE,
    BILLING,
    OTHER
} Category;

typedef enum
{
    LOW,
    MEDIUM,
    HIGH
} Priority;

typedef struct
{
    char username[USERNAME_SIZE];
    char password[PASSWORD_SIZE];
    Role role;
} User;

typedef struct
{
    int id;
    char reason[REASON_SIZE];
    char description[DESCRIPTION_SIZE];
    Category category;
    Status status;
    time_t date;
    char submitting_user[USERNAME_SIZE];
    time_t resolve_time;
    time_t time;
    Priority priority;
    Role client_role;
} Ticket;

void signup(User *users, int *user_count, char *username, char *password, Role role)
{
    char username[USERNAME_SIZE];
    char password[PASSWORD_SIZE];
    int has_upper = 0, has_lower = 0, has_digit = 0, has_symbol = 0, has_space = 0;

    if (*user_count >= MAX_USERS)
    {
        printf("Maximum number of users reached.\n");
        return;
    }

    printf("Enter username: ");
    fgets(username, USERNAME_SIZE, stdin);
    strcpy(users[*user_count].username, username);
    printf("Enter password: ");
    fgets(password, PASSWORD_SIZE, stdin);
    strcpy(users[*user_count].password, password);
    users[*user_count].role = USER;
    for (int i = 0; i < strlen(password[i]); i++)
    {
        if (islower(password[i]))
        {
            has_lower = 1;
        }
        else if (isupper(password[i]))
        {
            has_upper = 1;
        }
        else if (isdigit(password[i]))
        {
            has_digit = 1;
        }
        else if (ispunct(password[i]))
        {
            has_symbol = 1;
        }
        else if (isspace(password[i]))
        {
            has_space = 1;
        }
    }
    int valid = 1;
    if (has_upper == 0 || has_lower == 0 || has_digit == 0 || has_symbol == 0 || has_space == 0)
    {
        valid = 0;
        printf("Password must contain at least one uppercase letter, one lowercase letter, one digit, one symbol and no spaces.\n");
    }

    if (valid == 1)
    {
        (*user_count)++;
        printf("Sign up successful.\n");
    }
}

void login(int user_count, User users_array[], char username, char password, char submitting_user)
{
    char username[USERNAME_SIZE];
    char password[PASSWORD_SIZE];
    int i;
    printf("Enter username: ");
    fgets(username, USERNAME_SIZE, stdin);
    printf("Enter password: ");
    fgets(password, PASSWORD_SIZE, stdin);
    for (i = 0; i < user_count; i++)
    {
        if (users_array[i].role == ADMIN)
        {
            if (strcmp(username, users_array[i].username) == 0 && strcmp(password, users_array[i].password) == 0)
            {
                printf("Admin login successful.\n");
            }
            else if (users_array[i].role == AGENT)
            {
                if (strcmp(username, users_array[i].username) == 0 && strcmp(password, users_array[i].password) == 0)
                {
                    printf("Agent login successful.\n");
                }
            }
            else if (users_array[i].role == USER)
            {
                if (strcmp(username, users_array[i].username) == 0 && strcmp(password, users_array[i].password) == 0)
                {
                    printf("User login successful.\n");
                }
            }
        }
    }

    if (i == user_count)
        printf("Invalid username or password.\n");
    submitting_user = users_array[i].username;
    return;
}
void create_ticket(Ticket *tickets, int *ticket_count, char *submitting_user, Role client_role)
{
    char reason[REASON_SIZE];
    char description[DESCRIPTION_SIZE];
    char client[CLIENT_SIZE];
    int category, priority, status;
    char client[25];
    char client_role;
    time_t date;
    srand(time(NULL));
    int id = (rand() % 1000) + 1;
    printf("Enter reason: ");
    fgets(reason, REASON_SIZE, stdin);
    printf("Enter description: ");
    fgets(description, DESCRIPTION_SIZE, stdin);
    printf("select category:\n");
    printf("1. Defective product\n");
    printf("2. Customer service\n");
    printf("3. Billing\n");
    printf("4. Other\n");
    scanf("%d", &category);
    tickets_array[*ticket_count].id = id;
    strcpy(tickets_array[*ticket_count].reason, reason);
    strcpy(tickets_array[*ticket_count].description, description);
    tickets_array[*ticket_count].category = category;
    tickets_array[*ticket_count].status = PENDING;
    if (strcspn(description, "money") == 0 || strcspn(description, "bill") == 0 || strcspn(description, "payment") == 0 || strcspn(description, "refund") == 0)
    {
        tickets_array[*ticket_count].priority = HIGH;
    }
    else if (strcspn(description, "delay") == 0 || strcspn(description, "delayed") == 0 || strcspn(description, "late") == 0)
    {
        tickets_array[*ticket_count].priority = MEDIUM;
    }
    else
    {
        tickets_array[*ticket_count].priority = LOW;
    }
    strcpy(tickets_array[*ticket_count].client, submitting_user);
    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(tickets_array[*ticket_count].client, users_array[i].username) == 0)
        {
            tickets_array[*ticket_count].client_role = users_array[i].role;
            break;
        }
    }

    tickets_array[*ticket_count].time = time(NULL);
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    strftime(tickets_array[*ticket_count].date, 100, "%d-%m-%Y %H:%M:%S", &tm);
    (*ticket_count)++;

    printf("Ticket created successfully.\n");
}
void manage_roles(User *users, int user_count)
{
    int choice;
    printf("Manage roles:\n");
    for (int i = 0; i < user_count; i++)
    {
        printf("User %d. %s\n", i + 1, users[i].username);
        printf("Role : %s\n", users[i].role == ADMIN ? "Admin" : users[i].role == AGENT ? "Agent"
                                                                                        : "User");
    }
    printf("Enter user number: ");
    scanf("%d", &choice);
    for (int i = 0; i < user_count; i++)
    {
        if (choice == i + 1)
        {
            printf("Enter new role: ");
            printf("1 Agent\n");
            printf("2. User\n");
            printf("3. Exit\n");
            scanf("%d", &choice);
            if (choice == 1)
            {
                users[i].role = AGENT;
            }
            else if (choice == 2)
            {
                users[i].role = USER;
            }
            else if (choice == 3)
            {
                break;
            }
            else
            {
                printf("Invalid choice.\n");
            }
            printf("Role changed successfully.\n");
            break;
        }
    }
}
void admin_panel(User *users, int user_count, Ticket *tickets, int ticket_count, char *submitting_user)
{
    int choice;
    printf("Admin panel:\n");
    printf("1. Create ticket\n");
    printf("2. View tickets\n");
    printf("4. View users\n");
    printf("5. View agents\n");
    printf("6. Manage roles\n");
    printf("7. View Stats\n");
    printf("8. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        create_ticket(tickets, ticket_count, submitting_user, AGENT);
        break;
    case 2:
        view_tickets(tickets, ticket_count, submitting_user);
        printf("do you want to proccess the ticket?\n");
        printf("1. Yes\n");
        printf("2. No\n");
        scanf("%d", &choice);
        if (choice == 1)
        {
            process_ticket(tickets, ticket_count);
        }
        else if (choice == 2)
        {
            break;
        }
        else
        {
            printf("Invalid choice.\n");
        }
        break;
    case 3:
        view_users(users, user_count);
        break;
    case 4:
        view_agents(users, user_count);
        break;
    case 5:
        view_agents(users, user_count);
        break;
    case 6:
        manage_roles(users, user_count);
        break;
    case 7:
        view_stats(tickets, ticket_count);
        break;
    case 8:
        exit(0);
        break;
    default:
        printf("Invalid choice.\n");
        break;
    }
}
void agent_panel(User *users, int user_count, Ticket *tickets, int ticket_count)
{
    int choice;
    printf("Agent panel:\n");
    printf("1. Create internal ticket\n");
    printf("2. Create external ticket\n");
    printf("3. View tickets\n");
    printf("4. View users\n");
    printf("5.Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        create_internal_ticket(tickets, ticket_count);
        break;
    case 2:
        create_external_ticket(tickets, ticket_count);
        break;
    case 3:
        view_tickets(tickets, ticket_count, submitting_user);
        break;
    case 4:
        view_users(users, user_count);
        break;
    case 5:
        exit(0);
        break;
    default:
        printf("Invalid choice.\n");
        break;
    }
}
void view_tickets(Ticket *tickets, int ticket_count, char *submitting_user)
{
    for (int i = 0; i < ticket_count; i++)
    {
        if (tickets[i].client == submitting_user)
        {
            printf("Ticket %d. %s\n", i + 1, tickets[i].reason);
            printf("Description: %s\n", tickets[i].description);
            printf("Category: %s\n", tickets[i].category == 1 ? "Defective product" : tickets[i].category == 2 ? "Customer service"
                                                                                  : tickets[i].category == 3   ? "Billing"
                                                                                                               : "Other");
            printf("Status: %s\n", tickets[i].status == 1 ? "Pending" : tickets[i].status == 2 ? "In progress"
                                                                    : tickets[i].status == 3   ? "Resolved"
                                                                                               : "Closed");
            printf("Priority: %s\n", tickets[i].priority == 1 ? "High" : tickets[i].priority == 2 ? "Medium"
                                                                                                  : "Low");
            printf("Client: %s\n", tickets[i].client);
            printf("Date: %s\n", tickets[i].date);
            printf("\n");
        }
        else if (tickets[i].client == submitting_user && tickets_array == AGENT)
        {
        }
    }
}
void view_users(User *users, int user_count)
{
    for (int i = 0; i < user_count; i++)
    {
        printf("User %d. %s\n", i + 1, users[i].username);
        printf("Role: %s\n", users[i].role == ADMIN ? "Admin" : users[i].role == AGENT ? "Agent"
                                                                                       : "User");
        printf("\n");
    }
}
void view_agents(User *users, int user_count)
{
    for (int i = 0; i < user_count; i++)
    {
        if (users[i].role == AGENT)
        {
            printf("Agent %d. %s\n", i + 1, users[i].username);
            printf("\n");
        }
    }
}
void process_ticket(Ticket *tickets, int ticket_count)
{
    int id;
    int choice;
    printf("Enter ticket id: ");
    scanf("%d", &id);
    for (int i = 0; i < ticket_count; i++)
    {
        if (tickets[i].id != id)
        {
            printf("Ticket not found.\n");
            return;
        }
        else if (tickets[i].id == id)
        {
            tickets[i].status = IN_PROGRESS;
            printf("Ticket description is: %s\n", tickets[i].description);
            printf("Ticket priority is: %s\n", tickets[i].priority == HIGH ? "High" : tickets[i].priority == MEDIUM ? "Medium"
                                                                                                                    : "Low");
            printf("Ticket category is: %s\n", tickets[i].category);
            printf("Ticket date is: %s\n", tickets[i].date);

            printf("enter the new description: ");
            fgets(tickets[i].description, DESCRIPTION_SIZE, stdin);
            tickets[i].description[strcspn(tickets[i].description, "\n")] = '\0';
            printf("Ticket description is: %s\n", tickets[i].description);

            printf("enter the new priority: ");
            printf("1. High\n2. Medium\n3. Low\n");
            scanf("%d", &choice);
            if (choice == 1)
            {
                tickets[i].priority = HIGH;
            }
            else if (choice == 2)
            {
                tickets[i].priority = MEDIUM;
            }
            else if (choice == 3)
            {
                tickets[i].priority = LOW;
            }
            else
            {
                printf("Invalid choice.\n");
            }

            printf("Ticket priority is: %s\n", tickets[i].priority == HIGH ? "High" : tickets[i].priority == MEDIUM ? "Medium"
                                                                                                                    : "Low");

            printf("enter the new status: ");
            printf("1. Pending\n2. In progress\n3. Resolved\n");
            scanf("%d", &choice);
            if (choice == 1)
            {
                tickets[i].status = PENDING;
            }
            else if (choice == 2)
            {
                tickets[i].status = IN_PROGRESS;
            }
            else if (choice == 3)
            {
                tickets[i].status = RESOLVED;
            }
            else
            {
                printf("Invalid choice.\n");
            }
            printf("Ticket status is: %s\n", tickets[i].status);
            break;
        }
        if (tickets[i].status == RESOLVED)
        {
            printf("Ticket resolved successfully.\n");
            tickets_array[i].resolve_time = time(NULL);
            break;
        }
        break;
    }
}
void view_stats(Ticket *tickets, int ticket_count)
{
    int total_tickets = ticket_count;
    int total_users = user_count;
    int number_of_agents = 0;
    int open_tickets = 0;
    int in_progress_tickets = 0;
    int resolved_tickets = 0;
    for (int i = 0; i < ticket_count; i++)
    {
        if (tickets[i].status == PENDING)
        {
            open_tickets++;
        }
        else if (tickets[i].status == IN_PROGRESS)
        {
            in_progress_tickets++;
        }
        else if (tickets[i].status == RESOLVED)
        {
            resolved_tickets++;
        }
    }
    for (int i = 0; i < user_count; i++)
    {
        if (users_array[i].role == AGENT)
        {
            number_of_agents++;
        }
    }
    printf("Statistics:\n");
    printf("total number of agents: %d\n", number_of_agents);
    printf("Total number of users: %d\n", total_users);
    printf("Total number of tickets: %d\n", total_tickets);
    printf("Number of open tickets: %d\n", open_tickets);
    printf("Number of in progress tickets: %d\n", in_progress_tickets);
    printf("Number of resolved tickets: %d\n", resolved_tickets);
    double avg_time = 0.0;
    int resolved_tickets_count = 0;
    for (int i = 0; i < ticket_count; i++)
    {
        if (tickets[i].status == RESOLVED)
        {
            avg_time += tickets[i].resolve_time - tickets[i].time;
            resolved_tickets_count++;
        }
    }
    if (resolved_tickets_count > 0)
    {
        avg_time /= resolved_tickets_count;
    }
    printf("Average time to resolve tickets: %.2f seconds\n", avg_time / 60.0);
}

void user_panel(User *users, int user_count, Ticket *tickets, int ticket_count, char *submitting_user, Role client_role)
{
    int choice;
    printf("User panel:\n");
    printf("1. Create ticket\n");
    printf("2. View your tickets\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        create_ticket(tickets, ticket_count, submitting_user, client_role);
        break;
    case 2:
    }
}
#endif