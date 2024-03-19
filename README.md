# C++ PostgreSQL CRUD Application

## Overview

A simple C++ program that interfaces with a PostgreSQL database to perform CRUD operations. Demo video: https://youtu.be/S6BGSoFSTR8

## Prerequisites

- PostgreSQL server installed and running on your host system.
- C++ development environment set up with C++11 support or later.
- libpqxx installed and accessible in your development environment. This is the official C++ API for interfacing with PostgreSQL.
- If running inside WSL2, ensure that PostgreSQL on the Windows host is configured to accept connections from the WSL2 environment.

## Compilation & Execution

Use the following command to compile the application:

```bash
g++ -o app main.cpp -lpqxx -lpq
./app <dbname> <username> <password>
```

## Important Notes on Database Connection
Try the following if the application hangs when trying to connect:
- Ensure that your PostgreSQL server is configured to listen on all interfaces or at least the interface that WSL2 will use to connect.
- Confirm that `pg_hba.conf` has appropriate entries to allow connections from the WSL2 subnet using MD5 password authentication.
- Ensure Firewall allows connections on PostgreSQL's port.
