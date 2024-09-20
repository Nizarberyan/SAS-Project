
#include "SAS-Functions.h"

int main()
{
    int initial_choice = 0;
    int user_choice = 0;
    int admin_choice = 0;
    users user_array[100] = {0};
    tickets ticket_array[100];
    int count = 1;
    int ticket_count = 0;
    char username[25];
    char password[25];
    int login_status = 0;

    user_array[0].username = strdup("admin");
    user_array[0].password = strdup("Admin@123");
    user_array[0].role = strdup("admin");

    for (int i = 1; i < 100; i++)
    {
        user_array[i].username = NULL;
        user_array[i].password = NULL;
    }

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
            SignUp(user_array);
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
                break;
            case 2:
            {
                printf("Admin login successful\n");
                printf("Welcome to the admin panel\n");
                printf("1. Display tickets\n");
                printf("2. Exit\n");
                printf("Enter your choice: ");
                scanf("%d", &admin_choice);
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
                break;
            }
            case 3:
                break;
            default:
                printf("Invalid choice\n");
                break;
            }
        }
    } while (initial_choice != 3);

    FreeTickets(ticket_array, ticket_count);
    FreeUsers(user_array, count);
    return 0;
}
