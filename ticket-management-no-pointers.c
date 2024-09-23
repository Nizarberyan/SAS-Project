
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#ifdef _WIN32
#include <process.h>
#include <windows.h>
#else
#include <unistd.h>
#include <pthread.h>
#endif

#define MAX_USERS 100
#define MAX_TICKETS 1000
#define USERNAME_SIZE 50
#define PASSWORD_SIZE 50
#define REASON_SIZE 100
#define DESCRIPTION_SIZE 500
#define TIME_SIZE 20

typedef enum
{
    ADMIN,
    AGENT,
    USER
} Role;

typedef enum
{
    DEFECTIVE_PRODUCT,
    CUSTOMER_SERVICE,
    BILLING,
    OTHER
} Category;

typedef enum
{
    PENDING,
    IN_PROGRESS,
    RESOLVED,
    CLOSED
} Status;

typedef enum
{
    HIGH,
    MEDIUM,
    LOW
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
    Priority priority;
    char logged_user[USERNAME_SIZE];
    Role client_role;
    time_t time;
    char date[TIME_SIZE];
    time_t resolve_time;
} Ticket;

// Global variables
User users[MAX_USERS];
int user_count = 1;
Ticket tickets[MAX_TICKETS];
int ticket_count = 0;
char logged_user[USERNAME_SIZE];
Role client_role;

void signup(char username[], char password[], Role role)
{
    if (user_count >= MAX_USERS)
    {
        printf("Maximum number of users reached.\n");
        return;
    }

    // Check if username already exists
    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(users[i].username, username) == 0)
        {
            printf("Username already exists. Please choose a different username.\n");
            return;
        }
    }

    int has_upper = 0, has_lower = 0, has_digit = 0, has_symbol = 0, has_space = 0;
    int password_length = strlen(password);

    // Check password requirements
    for (int i = 0; i < password_length; i++)
    {
        if (islower(password[i]))
            has_lower = 1;
        else if (isupper(password[i]))
            has_upper = 1;
        else if (isdigit(password[i]))
            has_digit = 1;
        else if (ispunct(password[i]))
            has_symbol = 1;
        else if (isspace(password[i]))
            has_space = 1;
    }

    // Check if password contains username
    char lower_username[USERNAME_SIZE], lower_password[PASSWORD_SIZE];
    strncpy(lower_username, username, USERNAME_SIZE - 1);
    lower_username[USERNAME_SIZE - 1] = '\0';
    strncpy(lower_password, password, PASSWORD_SIZE - 1);
    lower_password[PASSWORD_SIZE - 1] = '\0';

    for (int i = 0; lower_username[i]; i++)
        lower_username[i] = tolower(lower_username[i]);
    for (int i = 0; lower_password[i]; i++)
        lower_password[i] = tolower(lower_password[i]);

    int has_username = (strstr(lower_password, lower_username) != NULL);

    // Validate password
    if (has_upper == 0 || has_lower == 0 || has_digit == 0 || has_symbol == 0 || has_space == 1 || has_username == 1 || password_length < 8)
    {
        printf("Password must:\n");
        printf("- Contain at least one uppercase letter\n");
        printf("- Contain at least one lowercase letter\n");
        printf("- Contain at least one digit\n");
        printf("- Contain at least one symbol\n");
        printf("- Not contain any spaces\n");
        printf("- Not contain the username\n");
        printf("- Be at least 8 characters long\n");
        return;
    }

    // If all checks pass, create the new user
    strncpy(users[user_count].username, username, USERNAME_SIZE - 1);
    users[user_count].username[USERNAME_SIZE - 1] = '\0';
    strncpy(users[user_count].password, password, PASSWORD_SIZE - 1);
    users[user_count].password[PASSWORD_SIZE - 1] = '\0';
    users[user_count].role = role;

    user_count++;
    printf("Sign up successful.\n");
}

void login(char *logged_user)
{
    int i;
    char input_username[USERNAME_SIZE];
    char input_password[PASSWORD_SIZE];
    char retry_choice;

    // Loop to allow retrying the login if credentials are wrong
    while (1)
    {
        // Prompt for username and password
        printf("Enter username: ");
        fgets(input_username, USERNAME_SIZE, stdin);
        input_username[strcspn(input_username, "\n")] = '\0'; // Remove trailing newline

        printf("Enter password: ");
        fgets(input_password, PASSWORD_SIZE, stdin);
        input_password[strcspn(input_password, "\n")] = '\0'; // Remove trailing newline

        // Check credentials
        for (i = 0; i < user_count; i++)
        {
            if (strcmp(input_username, users[i].username) == 0 && strcmp(input_password, users[i].password) == 0)
            {
                // Copy the username of the logged-in user
                strncpy(logged_user, users[i].username, USERNAME_SIZE - 1);
                logged_user[USERNAME_SIZE - 1] = '\0'; // Ensure null-termination

                // Handle role-based login
                if (users[i].role == ADMIN)
                {
                    printf("Admin login successful.\n");
                    admin_panel(users, user_count, tickets, ticket_count, logged_user, client_role);
                }
                else if (users[i].role == AGENT)
                {
                    printf("Agent login successful.\n");
                    agent_panel(users, user_count, tickets, ticket_count, logged_user, client_role);
                }
                else if (users[i].role == USER)
                {
                    printf("User login successful.\n");
                    user_panel(users, user_count, tickets, ticket_count, logged_user, client_role);
                }

                // Set the client's role
                client_role = users[i].role;
                return; // Successful login, exit function
            }
        }

        // If login fails
        printf("Invalid username or password.\n");

        // Ask user if they want to retry
        printf("Would you like to retry? (y/n): ");
        retry_choice = getchar();
        while (getchar() != '\n')
            ; // Clear the newline left in the input buffer

        if (retry_choice != 'y' && retry_choice != 'Y')
        {
            return; // Exit login function if user doesn't want to retry
        }
    }
}

void create_ticket(char logged_user[], Role client_role, int *ticket_count)
{
    char reason[REASON_SIZE];
    char description[DESCRIPTION_SIZE];
    int category_choice;

    // Ensure ticket limit is not exceeded
    if (*ticket_count >= MAX_TICKETS)
    {
        printf("Maximum number of tickets reached.\n");
        return;
    }

    // Collect ticket details
    printf("Enter reason: ");
    fgets(reason, REASON_SIZE, stdin);
    reason[strcspn(reason, "\n")] = '\0'; // Remove trailing newline

    printf("Enter description: ");
    fgets(description, DESCRIPTION_SIZE, stdin);
    description[strcspn(description, "\n")] = '\0'; // Remove trailing newline

    // Prompt for category
    printf("Select category:\n");
    printf("1. Defective product\n");
    printf("2. Customer service\n");
    printf("3. Billing\n");
    printf("4. Other\n");
    scanf("%d", &category_choice);
    getchar(); // Clear the newline left by scanf

    // Create the new ticket
    srand(time(NULL));
    tickets[*ticket_count].id = (rand() % 1000) + 1;
    strncpy(tickets[*ticket_count].reason, reason, REASON_SIZE - 1);
    tickets[*ticket_count].reason[REASON_SIZE - 1] = '\0'; // Ensure null termination
    strncpy(tickets[*ticket_count].description, description, DESCRIPTION_SIZE - 1);
    tickets[*ticket_count].description[DESCRIPTION_SIZE - 1] = '\0'; // Ensure null termination
    tickets[*ticket_count].category = (Category)(category_choice - 1);
    tickets[*ticket_count].status = PENDING;

    // Set ticket priority based on description keywords
    if (strstr(description, "money") || strstr(description, "bill") ||
        strstr(description, "payment") || strstr(description, "refund"))
    {
        tickets[*ticket_count].priority = HIGH;
    }
    else if (strstr(description, "delay") || strstr(description, "delayed") ||
             strstr(description, "late"))
    {
        tickets[*ticket_count].priority = MEDIUM;
    }
    else
    {
        tickets[*ticket_count].priority = LOW;
    }

    // Log user and role info
    strncpy(tickets[*ticket_count].logged_user, logged_user, USERNAME_SIZE - 1);
    tickets[*ticket_count].logged_user[USERNAME_SIZE - 1] = '\0'; // Ensure null termination
    tickets[*ticket_count].client_role = client_role;

    // Get current time for ticket creation and resolution time
    time_t current_time = time(NULL);
    tickets[*ticket_count].time = current_time;
    tickets[*ticket_count].resolve_time = current_time; // Assuming resolve_time is set to the creation time for now

    // Convert time to a formatted string
    char date_str[TIME_SIZE];
    struct tm *tm_info = localtime(&current_time);
    strftime(date_str, sizeof(date_str), "%d-%m-%Y %H:%M:%S", tm_info);
    strncpy(tickets[*ticket_count].date, date_str, sizeof(tickets[*ticket_count].date) - 1);
    tickets[*ticket_count].date[sizeof(tickets[*ticket_count].date) - 1] = '\0'; // Ensure null termination

    // Increment ticket count
    (*ticket_count)++;

    printf("Ticket created successfully.\n");
}

void process_ticket(Ticket tickets[], int ticket_count, char logged_user[], Role client_role)
{
    if (ticket_count == 0)
    {
        printf("No tickets found.\n");
        return;
    }

    if (client_role == ADMIN || client_role == AGENT)
    {
        for (int i = 0; i < ticket_count; i++)
        {
            if (tickets[i].status == PENDING)
            {
                printf("Ticket ID: %d\n", tickets[i].id);
                printf("Reason: %s\n", tickets[i].reason);
                printf("Description: %s\n", tickets[i].description);
                printf("Category: %d\n", tickets[i].category);
                printf("Status: %d\n", tickets[i].status);
                printf("Priority: %d\n", tickets[i].priority);
                printf("Submitted by: %s\n", tickets[i].logged_user);
                printf("Submitter role: %d\n", tickets[i].client_role);
            }
        }

        int ticket_id;
        int choice;
        int new_status;
        int new_priority;
        int new_category;

        while (1)
        {
            printf("Enter ticket ID: ");
            if (scanf("%d", &ticket_id) != 1)
            {
                printf("Invalid ticket ID.\n");
                while (getchar() != '\n')
                {
                } // Clear input buffer
                continue;
            }

            printf("What do you want to do?\n");
            printf("1. Edit ticket\n");
            printf("2. Resolve ticket\n");
            printf("3. Delete ticket\n");
            printf("4. Cancel\n"); // Added a cancel option
            if (scanf("%d", &choice) != 1)
            {
                printf("Invalid choice.\n");
                while (getchar() != '\n')
                {
                } // Clear input buffer
                continue;
            }

            if (choice == 1)
            {
                for (int i = 0; i < ticket_count; i++)
                {
                    if (tickets[i].id == ticket_id)
                    {
                        printf("Enter new reason: ");
                        fgets(tickets[i].reason, REASON_SIZE, stdin);
                        tickets[i].reason[strcspn(tickets[i].reason, "\n")] = '\0'; // Remove newline

                        printf("Enter new description: ");
                        fgets(tickets[i].description, DESCRIPTION_SIZE, stdin);
                        tickets[i].description[strcspn(tickets[i].description, "\n")] = '\0'; // Remove newline

                        printf("Enter new category: ");
                        printf("1. Defective product\n");
                        printf("2. Customer service\n");
                        printf("3. Billing\n");
                        printf("4. Other\n");
                        if (scanf("%d", &new_category) != 1)
                        {
                            printf("Invalid category.\n");
                            while (getchar() != '\n')
                            {
                            } // Clear input buffer
                            continue;
                        }
                        tickets[i].category = (Category)(new_category - 1);

                        printf("Enter new status: ");
                        printf("1. PENDING\n");
                        printf("2. IN_PROGRESS\n");
                        printf("3. RESOLVED\n");
                        printf("4. CLOSED\n");
                        if (scanf("%d", &new_status) != 1)
                        {
                            printf("Invalid status.\n");
                            while (getchar() != '\n')
                            {
                            } // Clear input buffer
                            continue;
                        }
                        tickets[i].status = (Status)(new_status - 1);

                        printf("Enter new priority: ");
                        printf("1. HIGH\n");
                        printf("2. MEDIUM\n");
                        printf("3. LOW\n");
                        if (scanf("%d", &new_priority) != 1)
                        {
                            printf("Invalid priority.\n");
                            while (getchar() != '\n')
                            {
                            } // Clear input buffer
                            continue;
                        }
                        tickets[i].priority = (Priority)(new_priority - 1);

                        printf("Ticket updated successfully.\n");
                        printf("Do you want to mark it as resolved or closed the ticket?\n");
                        printf("1. RESOLVED\n");
                        printf("2. CLOSED\n");
                        int answer;
                        if (scanf("%d", &answer) != 1)
                        {
                            printf("Invalid choice.\n");
                            while (getchar() != '\n')
                            {
                            } // Clear input buffer
                            continue;
                        }
                        if (answer == 1)
                        {
                            tickets[i].status = RESOLVED;
                            tickets[i].resolve_time = time(NULL);
                        }
                        else if (answer == 2)
                        {
                            tickets[i].status = CLOSED;
                            tickets[i].resolve_time = time(NULL);
                        }
                        break; // Exit the loop after successful update
                    }
                }
            }
            else if (choice == 2)
            {
                for (int i = 0; i < ticket_count; i++)
                {
                    if (tickets[i].id == ticket_id)
                    {
                        tickets[i].status = RESOLVED;
                        tickets[i].resolve_time = time(NULL);
                        printf("Ticket resolved successfully.\n");
                        break; // Exit the loop after resolving
                    }
                }
            }
            else if (choice == 3)
            {
                for (int i = 0; i < ticket_count; i++)
                {
                    if (tickets[i].id == ticket_id)
                    {
                        tickets[i].status = CLOSED;
                        printf("Ticket deleted successfully.\n");
                        break; // Exit the loop after deleting
                    }
                }
            }
            else if (choice == 4)
            {
                // Cancel the operation and return
                printf("Operation canceled.\n");
                return;
            }
            else
            {
                printf("Invalid choice.\n");
            }
        }
    }
}
void admin_panel(User users[], int user_count, Ticket tickets[], int ticket_count, char logged_user[], Role client_role)
{
    int choice;

    do
    {
        printf("Admin panel:\n");
        printf("1. Create ticket\n");
        printf("2. View tickets\n");
        printf("3. View users\n");
        printf("4. View agents\n");
        printf("5. View statistics\n");
        printf("6. Manage users\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid choice. Please enter a number.\n");
            while (getchar() != '\n')
            {
            } // Clear input buffer
            continue;
        }

        switch (choice)
        {
        case 1:
            create_ticket(logged_user, client_role, ticket_count);
            break;
        case 2:
            view_tickets(tickets, ticket_count, logged_user);
            break;
        case 3:
            view_users(users, user_count);
            break;
        case 4:
            view_agents(users, user_count);
            break;
        case 5:
            view_stats(tickets, ticket_count);
            break;
        case 6:
            edit_user(users, user_count, logged_user); // Renamed manage_users
            break;
        case 7:
            exit(0);
        default:
            printf("Invalid choice.\n");
            break;
        }
    } while (choice != 7);
}
void agent_panel(User users[], int user_count, Ticket tickets[], int ticket_count, char logged_user[], Role client_role)
{
    int choice;

    printf("Agent panel:\n");
    printf("1. Create internal ticket\n");
    printf("2. Create external ticket\n");
    printf("3. View tickets\n");
    printf("4. View users\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");

    if (scanf("%d", &choice) != 1)
    {
        printf("Invalid choice. Please enter a number.\n");
        while (getchar() != '\n')
        {
        } // Clear input buffer
        return;
    }

    switch (choice)
    {
    case 1:
        create_internal_ticket(logged_user, client_role, ticket_count); // Renamed function
        break;
    case 2:
        create_external_ticket(logged_user, client_role, ticket_count); // Renamed function
        break;
    case 3:
        view_tickets(tickets, ticket_count, logged_user);
        break;
    case 4:
        view_users(users, user_count);
        break;
    case 5:
        exit(0);
    default:
        printf("Invalid choice.\n");
        break;
    }
}

void user_panel(User users[], int user_count, Ticket tickets[], int ticket_count, char logged_user[], Role client_role)
{
    int choice;

    printf("User panel:\n");
    printf("1. Create ticket\n");
    printf("2. View your tickets\n");
    printf("3. Edit your ticket\n");
    printf("4. Delete your ticket\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");

    if (scanf("%d", &choice) != 1)
    {
        printf("Invalid choice. Please enter a number.\n");
        while (getchar() != '\n')
        {
        } // Clear input buffer
        return;
    }

    switch (choice)
    {
    case 1:
        create_ticket(logged_user, client_role, ticket_count);
        break;
    case 2:
        view_tickets(tickets, ticket_count, logged_user);
        break;
    case 3:
        user_edit_ticket(tickets, ticket_count);
        break;
    case 4:
        user_delete_ticket(tickets, ticket_count);
        break;
    case 5:
        exit(0);
    default:
        printf("Invalid choice.\n");
        break;
    }
}
void user_edit_ticket(Ticket tickets[], int ticket_count)
{
    if (ticket_count == 0)
    {
        printf("No tickets found.\n");
        return;
    }

    int ticket_id;
    printf("Enter the ID of the ticket you want to edit: ");

    if (scanf("%d", &ticket_id) != 1)
    {
        printf("Invalid ticket ID.\n");
        while (getchar() != '\n')
        {
        } // Clear input buffer
        return;
    }

    for (int i = 0; i < ticket_count; i++)
    {
        if (tickets[i].id == ticket_id)
        {
            printf("Enter the new reason: ");
            fgets(tickets[i].reason, REASON_SIZE, stdin);
            tickets[i].reason[strcspn(tickets[i].reason, "\n")] = '\0'; // Remove newline

            printf("Enter the new description: ");
            fgets(tickets[i].description, DESCRIPTION_SIZE, stdin);
            tickets[i].description[strcspn(tickets[i].description, "\n")] = '\0'; // Remove newline

            printf("Enter the new category: ");
            printf("1. Defective product\n");
            printf("2. Customer service\n");
            printf("3. Billing\n");
            printf("4. Other\n");

            int category_choice;
            if (scanf("%d", &category_choice) != 1)
            {
                printf("Invalid category choice.\n");
                while (getchar() != '\n')
                {
                } // Clear input buffer
                return;
            }

            switch (category_choice)
            {
            case 1:
                tickets[i].category = 0;
                break;
            case 2:
                tickets[i].category = 1;
                break;
            case 3:
                tickets[i].category = 2;
                break;
            case 4:
                tickets[i].category = 3;
                break;
            default:
                printf("Invalid category choice.\n");
                return;
            }

            printf("Ticket edited successfully.\n");
            return;
        }
    }
}
void user_delete_ticket(Ticket tickets[], int ticket_count)
{
    int ticket_id;

    printf("Enter the ID of the ticket you want to delete: ");

    if (scanf("%d", &ticket_id) != 1)
    {
        printf("Invalid ticket ID.\n");
        while (getchar() != '\n')
        {
        } // Clear input buffer
        return;
    }

    for (int i = 0; i < ticket_count; i++)
    {
        if (tickets[i].id == ticket_id)
        {
            printf("Are you sure you want to delete ticket %d? (y/n): ", ticket_id);
            char confirmation;
            if (scanf(" %c", &confirmation) != 1 || (confirmation != 'y' && confirmation != 'Y'))
            {
                printf("Deletion canceled.\n");
                return;
            }

            for (int j = i; j < ticket_count - 1; j++)
            {
                tickets[j] = tickets[j + 1];
            }
            ticket_count--;
            printf("Ticket deleted successfully.\n");
            return;
        }
    }

    printf("Ticket with ID %d not found.\n", ticket_id);
}
void view_tickets(Ticket tickets[], int ticket_count, char *submitting_user)
{
    if (ticket_count == 0)
    {
        printf("No tickets found.\n");
        return;
    }
    if (client_role == ADMIN || client_role == AGENT)
    {
        for (int i = 0; i < ticket_count; i++)
        {
            printf("id : %d\n", tickets[i].id);
            printf("reason : %s\n", tickets[i].reason);
            printf("description : %s\n", tickets[i].description);
            printf("category : %s\n", tickets[i].category == 0 ? "Defective product" : tickets[i].category == 1 ? "Customer service"
                                                                                   : tickets[i].category == 2   ? "Billing"
                                                                                                                : "Other");
            printf("status : %s\n", tickets[i].status == 0 ? "PENDING" : tickets[i].status == 1 ? "IN_PROGRESS"
                                                                     : tickets[i].status == 2   ? "RESOLVED"
                                                                                                : "CLOSED");
            printf("priority : %s\n", tickets[i].priority == 0 ? "HIGH" : tickets[i].priority == 1 ? "MEDIUM"
                                                                                                   : "LOW");
            printf("Submitted by : %s\n", tickets[i].logged_user);
            printf("Date : %s\n", tickets[i].date);
            printf("\n");
        }
    }
    else if (client_role == USER)
    {
        for (int i = 0; i < ticket_count; i++)
        {
            if (tickets[i].logged_user == logged_user)
            {
                printf("id : %d\n", tickets[i].id);
                printf("reason : %s\n", tickets[i].reason);
                printf("description : %s\n", tickets[i].description);
                printf("category : %s\n", tickets[i].category == 0 ? "Defective product" : tickets[i].category == 1 ? "Customer service"
                                                                                       : tickets[i].category == 2   ? "Billing"
                                                                                                                    : "Other");
                printf("status : %s\n", tickets[i].status == 0 ? "PENDING" : tickets[i].status == 1 ? "IN_PROGRESS"
                                                                         : tickets[i].status == 2   ? "RESOLVED"
                                                                                                    : "CLOSED");
                printf("priority : %s\n", tickets[i].priority == 0 ? "HIGH" : tickets[i].priority == 1 ? "MEDIUM"
                                                                                                       : "LOW");
                printf("Submitted by : You\n");
                printf("Date : %s\n", tickets[i].date);
            }
        }
    }
    else
    {
        printf("Invalid role.\n");
        return;
    }
}
void view_users(User users[], int user_count)
{
    if (user_count == 0)
    {
        printf("No users  found :(");
        return;
    }
    if (client_role == ADMIN)
    {
        for (int i = 0; i < user_count; i++)
        {
            printf("username: %s", users[i].username);
            printf("Role: %s", users[i].role == 0 ? "Admin" : users[i].role == 1 ? "Agent"
                                                                                 : "User");
        }
    }
}
void manage_users(User users[], int user_count, char *logged_user)
{
    int choice;
    char username_choice[USERNAME_SIZE];
    if (logged_user == ADMIN)
    {
        do
        {
            view_users;
            printf("Enter a username to manage: ");
            fgets(username_choice, USERNAME_SIZE, stdin);
            for (int i = 0; i < user_count; i++)
            {
                if (strcmp(users[i].username, username_choice) == 0)
                {
                    printf("What would you like to do? \n");
                    printf("1. Change role\n");
                    printf("2. Change username\n");
                    printf("3. Change password\n");
                    printf("4. Delete user\n");
                    printf("5. Exit\n");
                    printf("Enter your choice: ");
                    scanf("%d", &choice);
                }
                switch (choice)
                {
                case 1: // change role
                    printf("Enter new role: \n");
                    printf("1. Admin\n");
                    printf("2. Agent\n");
                    printf("3. User\n");
                    scanf("%d", &choice);
                    if (choice == 1)
                    {
                        users[i].role = ADMIN;
                    }
                    else if (choice == 2)
                    {
                        users[i].role = AGENT;
                    }
                    else
                    {
                        users[i].role = USER;
                    }
                    break;
                case 2: // change username
                    printf("Enter new username: \n");
                    scanf("%s", &users[i].username);
                    break;
                case 3: // change password
                    printf("Enter new password: \n");
                    scanf("%s", &users[i].password);
                    break;
                case 4: // delete user
                    printf("Are you sure you want to delete this user? \n");
                    printf("1. Yes\n");
                    printf("2. No\n");
                    scanf("%d", &choice);
                    if (choice == 1)
                    {
                        for (int j = i; j < user_count - 1; j++)
                        {
                            users[j] = users[j + 1];
                        }
                        user_count--;
                        printf("User deleted.\n");
                        break;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        } while (choice != 5);
    }
    else
    {
        printf("You are not supposed to be here you silly goose.\n");
        exit(0);
    }
}
void view_agents(User users[], int user_count)
{
    if (user_count == 0)
    {
        printf("No users  found :(");
        return;
    }
    if (client_role == AGENT)
    {
        for (int i = 0; i < user_count; i++)
        {
            printf("username: %s", users[i].username);
            printf("Role: %s", users[i].role == 0 ? "Admin" : users[i].role == 1 ? "Agent"
                                                                                 : "User");
        }
    }
}
void view_stats(Ticket tickets[], int ticket_count)
{
    int pending_tickets = 0;
    int in_progress_tickets = 0;
    int resolved_tickets = 0;
    int closed_tickets = 0;
    int average_resolve_time = 0;
    int total_resolve_time = 0;

    if (user_count == 0)
    {
        printf("No users  found :(");
        return;
    }
    if (client_role == ADMIN)
    {
        printf("Total number of users: %d\n", user_count);
        printf("Total number of tickets: %d\n", ticket_count);
        for (int i = 0; i < ticket_count; i++)
        {
            if (tickets[i].status == 0)
            {
                pending_tickets++;
            }
            else if (tickets[i].status == 1)
            {
                in_progress_tickets++;
            }
            else if (tickets[i].status == 2)
            {
                resolved_tickets++;
            }
            else if (tickets[i].status == 3)
            {
                closed_tickets++;
            }
        }
        printf("Total number of open tickets: %d\n", pending_tickets);
        printf("Total number of in progress tickets: %d\n", in_progress_tickets);
        printf("Total number of resolved tickets: %d\n", resolved_tickets);
        printf("Total number of closed tickets: %d\n", closed_tickets);
        for (int i = 0; i < ticket_count; i++)
        {
            if (tickets[i].status == 2)
            {
                total_resolve_time += tickets[i].resolve_time;
            }
        }
        average_resolve_time = total_resolve_time / ticket_count;
    }
}
void daily_report(Ticket tickets[], int ticket_count)
{
    int in_progress_tickets = 0;
    int resolved_tickets = 0;
    int closed_tickets = 0;
    int pending_tickets = 0;
    int average_resolve_time = 0;
    int number_of_agents = 0;
    int number_of_users = 0;
    int number_of_admins = 0;
    int total_resolve_time = 0;
    time_t now;
    struct tm *local_time;
    FILE *file;

    // Get current time
    time(&now);
    local_time = localtime(&now);

    file = fopen("daily_report.txt", "w");

    if (file == NULL)
    {
        printf("Failed to open file for writing.\n");
        return;
    }
    for (int i = 0; i < ticket_count; i++)
    {
        if (tickets[i].status == 0)
        {
            pending_tickets++;
        }
        else if (tickets[i].status == 1)
        {
            in_progress_tickets++;
        }
        else if (tickets[i].status == 2)
        {
            resolved_tickets++;
        }
        else if (tickets[i].status == 3)
        {
            closed_tickets++;
        }
    }
    for (int i = 0; i < user_count; i++)
    {
        if (users[i].role == 0)
        {
            number_of_admins++;
        }
        else if (users[i].role == 1)
        {
            number_of_agents++;
        }
        else
        {
            number_of_users++;
        }
    }
    total_resolve_time = 0;
    for (int i = 0; i < ticket_count; i++)
    {
        total_resolve_time += tickets[i].resolve_time;
    }
    if (ticket_count > 0)
    {
        average_resolve_time = (total_resolve_time / ticket_count) / 60;
    }
    else
    {
        average_resolve_time = 0;
    }

    fprintf(file, "Ticket Management System Daily Report\n");
    fprintf(file, "Date: %s\n", asctime(local_time));
    fprintf(file, "Total number of tickets: %d\n", ticket_count);
    fprintf(file, "Total number of users: %d\n", user_count);
    fprintf(file, "Total number of admins: %d\n", number_of_admins);
    fprintf(file, "Total number of agents: %d\n", number_of_agents);
    fprintf(file, "Total number of users: %d\n", number_of_users);
    fprintf(file, "Total number of open tickets: %d\n", pending_tickets);
    fprintf(file, "Total number of in progress tickets: %d\n", in_progress_tickets);
    fprintf(file, "Total number of resolved tickets: %d\n", resolved_tickets);
    fprintf(file, "Total number of closed tickets: %d\n", closed_tickets);
    fprintf(file, "Average resolve time: %d hours\n", average_resolve_time);
    fclose(file);
}
void start_daily_report_scheduler()
{
    while (1)
    {
        daily_report(tickets, ticket_count);
        sleep(86400); // Sleep for 24 hours (86400 seconds)
    }
}
void initialize_daily_reports()
{
#ifdef _WIN32
    _beginthread(start_daily_report_scheduler, 0, NULL);
#endif
}
int main()
{
    initialize_daily_reports();

    // Set up admin account
    strcpy(users[0].password, "Admin@123");
    strcpy(users[0].username, "admin");
    users[0].role = ADMIN;

    int choice;

    while (1)
    { // Loop to allow the user to interact multiple times
        printf("\nTicket Management System\n");
        printf("1. Login\n");
        printf("2. Signup\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        { // Input validation
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n')
                ; // Clear the buffer
            continue;
        }

        getchar(); // Clear newline character after the number input

        char username[USERNAME_SIZE];
        char password[PASSWORD_SIZE];

        switch (choice)
        {
        case 1:
            printf("Enter username: ");
            fgets(username, USERNAME_SIZE, stdin);
            username[strcspn(username, "\n")] = 0; // Remove trailing newline

            printf("Enter password: ");
            fgets(password, PASSWORD_SIZE, stdin);
            password[strcspn(password, "\n")] = 0; // Remove trailing newline

            login(username, password, logged_user);
            break;

        case 2:
            printf("Enter a new username: ");
            fgets(username, USERNAME_SIZE, stdin);
            username[strcspn(username, "\n")] = 0; // Remove trailing newline

            printf("Enter a new password: ");
            fgets(password, PASSWORD_SIZE, stdin);
            password[strcspn(password, "\n")] = 0; // Remove trailing newline

            signup(username, password, USER);
            break;

        case 3:
            printf("Exiting the program...\n");
            exit(0);

        default:
            printf("Invalid choice. Please select 1, 2, or 3.\n");
        }
    }

    return 0;
}
