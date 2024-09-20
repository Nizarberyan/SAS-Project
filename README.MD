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

- **Sign-Up and Login**: Users can register with a username and password. Passwords must meet certain security criteria (including length, character types, etc.).
- **Password Validation**: Passwords are validated to include at least 8 characters, one uppercase, one lowercase, one digit, and one special character.
- **Admin Panel**: The hardcoded admin (`admin`, `Admin@123`) has special privileges to view and manage all tickets.
- **Ticket Creation**: Regular users can create tickets.
- **Ticket Viewing (Admin)**: Admin can view all user-submitted tickets.

---

## **Getting Started**

### **Prerequisites**

Before compiling and running the project, make sure you have the following installed:

- **GCC Compiler** (or any compatible C compiler)
- Basic knowledge of C

### **Installation**

1. Clone the repository:

   ```bash
   git clone https://github.com/your-username/SAS.git
   cd SAS
   ```

2. Compile the program:
   ```bash
   gcc main.c SAS-Functions.c -o SAS
   ```

---

## **Usage**

1. After compiling, run the program:

   ```bash
   ./SAS
   ```

2. You will be presented with three options:

   - **Sign Up**: Create a new user.
   - **Login**: Login using a registered user account.
   - **Exit**: Exit the program.

3. Admin users can log in with the default credentials to view all submitted tickets.

4. Regular users can log in, add tickets, and view their own tickets.

---

## **Code Structure**

- **`main.c`**: Contains the main logic for user interaction and program flow.
- **`SAS-Functions.h`**: Header file declaring the necessary functions.
- **`SAS-Functions.c`**: Core implementation of functions such as sign-up, login, ticket handling, and user/ticket management.
- **Data Structures**:
  - `users`: Holds information like username, password, and role.
  - `tickets`: Represents the user tickets.

---

## **Contributing**

Contributions are welcome! If you have suggestions or find any bugs, feel free to open an issue or submit a pull request. Please ensure that your code adheres to the project's coding standards.

---

## **License**

This project is licensed under the MIT License. Feel free to use, modify, and distribute it as you see fit.
