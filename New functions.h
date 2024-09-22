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

enum role
{
    admin,
    agent,
    user
} role;

enum status
{
    pending,
    in_progress,
    resolved,
    closed
} status;

enum category
{
    defective_product,
    customer_service,
    billing,
    other
} category;

enum priority
{
    low,
    medium,
    high
} priority;

typedef struct
{
    char username[USERNAME_SIZE];
    char password[PASSWORD_SIZE];
    char role[MAX_ROLE_SIZE];
} users;

typedef struct
{
    int id;
    char reason[REASON_SIZE];
    char description[DESCRIPTION_SIZE];
    char category[CATEGORY_SIZE];
    char status[STATUS_SIZE];
    time_t date;
    char priority[PRIORITY_SIZE];
    char client[CLIENT_SIZE];
} tickets;

int count = 0;
int ticket_count = 0;