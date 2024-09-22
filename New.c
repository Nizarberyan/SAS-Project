#include "New_functions.h"

int user_count = 0;
User users_array[MAX_USERS];
Ticket tickets_array[MAX_TICKETS];

int main()
{
    strcpy(users_array[0].username, "admin");
    strcpy(users_array[0].password, "Admin@123");
    users_array[0].role = ADMIN;
    char current_user[USERNAME_SIZE];
    return 0;
}