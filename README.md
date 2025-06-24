# Jorjek

Jorjek is a real-time, terminal-based chat application written in C, built specifically for Windows using socket programming. It features user authentication, an admin system, and real-time messaging. Admins can manage user accounts directly from the terminal.

## Background

I created Jorjek to strengthen my understanding of networking, client-server architecture, and user authentication. Throughout the project, I focused on building real-time communication from scratch while gradually adding more features like role management and system controls. This project serves as a solid base for future improvements like multithreading and secure data handling.

## Features

- **User Authentication:** Login and signup system to manage user sessions securely.
- **Admin Panel:** Admins can:
  - View all registered users
  - Delete users
  - Reset user passwords
  - Promote user to admin role
- **Real-Time Chat:** Terminal-based messaging between two clients using TCP sockets.
- **Role-Based Menus:** Different options for Admins and regular users.
- **Windows Optimized:** Built and tested for Windows terminals.

## Known Limitations

- Supports communication between up to two users at a time.
- No multithreading yet — cannot handle multiple chats concurrently.
- Basic file-based account storage (no database yet).
- Admin chat log viewing is a placeholder or a planned feature.

## Requirements

- Windows 10 or later
- GCC compiler (MinGW)

## Installation & Usage

Clone the repository:

```
git clone https://github.com/yourusername/jorjek.git
cd jorjek
```
```
gcc src\*.c -Iinclude -o build/jorjek.exe -lws2_32
```
```
./build/jorjek.exe
```
