# Jorjek

Jorjek is a **terminal-based singleplex chat application** written in C for Windows, featuring user authentication with password hashing, an admin system, and real-time messaging.

---

## Overview

Jorjek enables two users to communicate in real-time over a TCP connection. It supports user login/signup with hashed passwords, and an admin panel for managing accounts. Communication is **singleplex (half-duplex)**, meaning only one user can send messages at a time, requiring users to alternate turns to send and receive messages.

---

## Features

- **User Authentication:** Secure login and signup system with password hashing.
- **Admin Panel:** Admins can view users, delete accounts, reset passwords, and promote users to admin.
- **Singleplex Chat:** Users take turns sending messages; one sends while the other receives.
- **Role-Based Menus:** Different menu options for admins and regular users.
- **Windows Focused:** Uses Windows sockets (`winsock`) and terminal commands.
- **File-Based Storage:** User accounts stored in CSV files with hashed passwords.

---

## Limitations

- **Singleplex Communication:** Only one user can send messages at a time; users must wait their turn.
- **No Multithreading:** Chat communication is synchronous, limiting concurrency.
- **Two-User Support:** Designed for two-party chat only.
- **Basic File Storage:** No database integration.

---

## Requirements

- Windows 10 or later
- GCC compiler (MinGW recommended)

---

## Installation

   ```
   git clone https://github.com/yourusername/jorjek.git
   cd jorjek
   ```
   ```
   gcc src\*.c -Iinclude -o build/jorjek.exe -lws2_32 -lwinmm
   ```
   ```
   ./build/jorjek.exe
   ```



