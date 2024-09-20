````markdown
# **Simple Authentication System (SAS)**

Welcome to the **Simple Authentication System (SAS)** project! This C-based project provides a basic user authentication system, allowing for **user registration, login**, and **ticket management** with an admin panel for managing tickets.

---

## **Table of Contents**

1. [Project Overview](#project-overview)
2. [Features](#features)
3. [Getting Started](#getting-started)
   - [Prerequisites](#prerequisites)
   - [Installation](#installation)
4. [Usage](#usage)
5. [Code Structure](#code-structure)
6. [Contributing](#contributing)
7. [License](#license)

---

## **Project Overview**

The **Simple Authentication System** is a lightweight authentication system built in C that simulates basic functionality for user sign-up, login, and ticket submission. Admin users can manage tickets, while regular users can submit and view their own tickets.

The project includes the following core components:

- **User Registration & Login**
- **Password Validation**
- **Admin Privileges**
- **Ticket Management**

---

## **Features**

- **Sign-Up and Login**: Users can register with a username and password. Passwords must meet certain security criteria (including length and character types).
- **Password Validation**: Passwords must be at least 8 characters long and include uppercase, lowercase, digits, and special characters.
- **Admin Panel**: An admin account is hardcoded into the system for managing users and tickets.
- **Ticket Management**: Users can submit tickets, and admins can view and manage submitted tickets.

---

## **Getting Started**

### Prerequisites

- A C compiler like GCC
- Basic knowledge of C and file handling

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/Simple-Authentication-System.git
   ```
````

2. Navigate to the project directory:
   ```bash
   cd Simple-Authentication-System
   ```
3. Compile the code:
   ```bash
   gcc main.c SAS-Functions.c -o sas
   ```

---

## **Usage**

1. **Run the executable**:

   ```bash
   ./sas
   ```

2. **Choose from the available options**:

   - **1. Sign Up**: Register a new user.
   - **2. Login**: Login as an existing user or admin.
   - **3. Exit**: Quit the application.

3. **Admin Credentials**:

   - Username: `admin`
   - Password: `Admin@123`

4. **Admin Privileges**:
   Once logged in as admin, you can:
   - **Display Tickets**: View all submitted tickets.
   - **Exit**: Return to the main menu.

---

## **Code Structure**

- **main.c**: Contains the main logic for handling user interactions and control flow.
- **SAS-Functions.c**: Contains the core functionality like `SignUp`, `Login`, and `AddTicket`.
- **SAS-Functions.h**: The header file defining function prototypes and global variables.

---

## **Contributing**

Contributions are welcome! Please submit a pull request or open an issue to suggest improvements or report bugs.

---

## **License**

This project is licensed under the MIT License.

```

```
