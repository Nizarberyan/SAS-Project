#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>

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
    char date[TIME_SIZE];
    char logged_user[USERNAME_SIZE];
    time_t resolve_time;
    time_t time;
    Priority priority;
    Role client_role;
} Ticket;

void signup(char username[], char password[], Role role);
void login(char username[], char password[], char logged_user[]);
void create_ticket(char logged_user[], Role client_role, int ticket_count);
void proccess_ticket(Ticket tickets[], int ticket_count, char logged_user[], Role client_role);
void admin_panel(User users[], int user_count, Ticket tickets[], int ticket_count, char logged_user[], Role client_role);
void agent_panel(User users[], int user_count, Ticket tickets[], int ticket_count, char logged_user[], Role client_role);
void user_panel(User users[], int user_count, Ticket tickets[], int ticket_count, char logged_user[], Role client_role);
void user_edit_ticket(Ticket tickets[], int ticket_count);
void view_tickets(Ticket tickets[], int ticket_count, char *submitting_user);
void view_users(User users[], int user_count);
void manage_users(User users[], int user_count, char *logged_user);
void view_agents(User users[], int user_count);
void view_stats(Ticket tickets[], int ticket_count);
void daily_report(Ticket tickets[], int ticket_count);
void start_daily_report_scheduler();
void initialize_daily_reports();
void user_delete_ticket(Ticket tickets[], int ticket_count);