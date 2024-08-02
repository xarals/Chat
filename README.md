# Chat

This is a C++ application for information exchange between users, featuring a server-client architecture and a robust role-based system.

## Features

- **User Authentication**: Users can register and log in to the server.
- **Role System**: Different user roles (user, moderator, admin, creator) with distinct permissions.
- **Role Management**: Higher roles can manage lower roles with commands like `/ban`, `/unban`, `/kick`, `/delete`, and `/addrole`.
- **MySQL Integration**: The server connects to a MySQL database to store registered users' information.
- **Real-time Communication**: The server handles real-time communication between connected clients.

## Usage

### Commands

- **Registration**: Register a new user.
    ```sh
    /registration <username> <password>
    ```

- **Login**: Log in to the server.
    ```sh
    /login <username> <password>
    ```

- **Role Management**:
    - Ban a user:
      ```sh
      /ban <username>
      ```
    - Unban a user:
      ```sh
      /unban <username>
      ```
    - Kick a user:
      ```sh
      /kick <username>
      ```
    - Delete a user:
      ```sh
      /delete <username>
      ```
    - Add a role to a user:
      ```sh
      /addrole <username> <role>
      ```
