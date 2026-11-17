# Zelia DB

A database project focused on understanding and implementing database concepts
from the ground up.

## Introduction

Zelia DB is an educational database-engineering project designed to make
storage concepts visible in a small, readable C++ codebase.

## Project Overview

Zelia DB is a C++17 storage-engine project exploring core database
architecture, data management, and disk storage concepts. It currently
implements fixed-size pages, page allocation, binary student-record
serialization, and a storage round-trip verification.

## Architecture

The current execution path is:

```text
main.cpp
  -> DiskManager
      -> database file
  -> Page
      -> serialized StudentRecord values
```

`Page` is a fixed-size 4096-byte byte array. `DiskManager` creates or opens a
binary database file, allocates pages, and reads or writes complete pages.
`record.cpp` serializes student IDs, ages, name lengths, and names into page
storage. The executable reopens the file to verify persistence.

## Features

- Fixed-size 4096-byte pages
- Page allocation, reading, and writing through a disk manager
- Binary serialization and deserialization of student records
- Persistence verification by reopening the database file

SQL parsing, query execution, indexes, transactions, and write-ahead logging
are planned but are not implemented yet.

## Technology Stack

- C++17
- CMake 3.20 or newer
- GitHub Actions for continuous integration

## Installation

### Requirements

- A C++17 compiler
- CMake 3.20 or newer
- MinGW or Visual Studio on Windows, or GCC/Clang on Linux and macOS

### Build with CMake

From the project root:

```powershell
cmake -S . -B build
cmake --build build
.\build\zeliadb.exe
```

On Linux or macOS, run `./build/zeliadb` instead.

### Build directly with MinGW on Windows

```powershell
g++ -std=c++17 -Wall -Wextra -Wpedantic -Isrc src/main.cpp src/storage/page.cpp src/storage/disk_manager.cpp src/storage/record.cpp -o zeliadb.exe
.\zeliadb.exe
```

## Usage

Running the executable creates a temporary `zeliadb.db` file, writes twelve
student records to a page, closes and reopens the file, reads the records back,
prints the results, and removes the temporary database file.

The storage API can also be used from another C++ program:

```cpp
#include "storage/disk_manager.h"
#include "storage/record.h"

zeliadb::DiskManager database("example.db");
const zeliadb::PageId page_id = database.allocate_page();
auto page = zeliadb::make_empty_page();
zeliadb::write_student_record(page, 0, {1, 21, "Harshita Kalita"});
database.write_page(page_id, page);
```

This example demonstrates page storage only; Zelia DB does not yet expose SQL
or a higher-level table API.

## Project Layout

```text
.
├── src/
│   ├── main.cpp
│   └── storage/
│       ├── disk_manager.cpp/.h
│       ├── page.cpp/.h
│       └── record.cpp/.h
├── .github/workflows/build.yml
├── CMakeLists.txt
└── README.md
```

## Testing

There is currently no standalone test suite. The executable performs the
available storage round-trip check: it writes twelve records, closes and
reopens the database file, reads the records, and reports the page and row
counts. Continuous integration performs the same build and execution check.

## Limitations

- No SQL parser or query executor
- No public insert, update, or delete API
- No indexes, buffer pool, transactions, or write-ahead logging
- Student records are the only supported record format
- The demonstration executable deletes its temporary database file on exit

## Future Improvements

1. Page headers and slotted-page records
2. Buffer pool
3. B+ tree index
4. SQL lexer and parser
5. Query execution and table scans
6. Transactions and write-ahead logging

## Contributing

Keep changes focused and preserve the storage round-trip behavior. Build the
project and run the executable before submitting a pull request. New storage
behavior should include a focused automated test when a test framework is
introduced.

## Attribution and Licensing

Zelia DB is an independently maintained derivative of the Astra DB project.
Refer to the original project for its underlying implementation and
development history.

Author: Harshita Kalita

This project is available under the [MIT License](LICENSE).
