# C Programming Summative Projects

This repository contains C Programming. The projects demonstrate basic programming concepts, including dynamic memory allocation, file handling, function pointers, callback functions, POSIX threads, and Arduino programming.

Each project is organized in its own folder and includes the source code, supporting files, and instructions for compiling or running the program.

---

## Project Demo

[Watch project Demo](https://canva.link/2n1vaqtvrwneirk)

## Projects

### Project 1: Smart Parking Lot Monitoring System

A smart parking system designed and simulated using Arduino and Tinkercad.

The system monitors four parking spaces using LEDs and push buttons. It keeps track of occupied and available spaces, prevents invalid parking operations, and displays parking information through the Serial Monitor.

The project also includes an EasyEDA schematic and PCB layout.

**Main concepts used:**

- Arduino programming
- Structures
- Pointers
- Dynamic memory
- Non-blocking programming using `millis()`
- Push-button input
- LED output
- Serial Monitor output
- PCB schematic and layout

[Open Project 1](question1/explanation.md)

---

### Project 2: Backup and Recovery Management System

A program designed to manage file backups and recovery operations.

The system allows users to create backups, view available backup records, restore saved files, and manage backup information. It uses file handling to store backup data and provides a menu-driven interface for interacting with the system.

**Main concepts used:**

- File handling
- Backup and recovery operations
- Structures
- Dynamic memory
- Input validation
- Error handling
- Bash scripting

[Open Project 2](question2/explanation.md)

---

### Project 3: Library Book Inventory Management System

A program used to manage library book records.

The program allows users to add, display, update, delete, search, and sort books. Book records are stored dynamically in memory and saved to a text file.

**Main concepts used:**

- Structures
- Dynamic memory using `malloc()`, `realloc()`, and `free()`
- File handling
- CRUD operations
- Manual searching
- Manual sorting
- Inventory reports
- Input validation
- Modular programming

[Open Project 3](question3/explanation.md)

---

### Project 4: Unit Conversion Toolkit

A program that performs different unit conversions and stores the results in a conversion history.

The program uses function pointers to select conversion functions and callback functions to process, filter, and compare conversion records.

**Main concepts used:**

- Function pointers
- Callback functions
- Dynamic memory
- File handling
- Conversion history
- Searching
- Sorting
- Input validation
- Modular programming

[Open Project 4](question4/explanation.md)

---

### Project 5: Multi-threaded File Processing System

A program that uses POSIX threads to process multiple text files at the same time.

Each thread analyzes one file and counts its lines, words, and characters. The results are saved in separate output files.

**Main concepts used:**

- POSIX threads
- Concurrent file processing
- File handling
- Dynamic memory
- Command-line arguments
- Modular programming
- Error handling

[Open Project 5](question5/explanation.md)

---

## Repository Structure

```text
C-Programming-Summative/
│
├── README.md
│
├── LICENSE
│
├── .gitignore
│
├── question1/  # smart parking monitoring
│
├── question3/  # library inventory management
│
├── question4/  # calc. with conversion history
│
└── question5/  # multithreading with pthreads
```
---

## Author

**Monica Dhieu**

