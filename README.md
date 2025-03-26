# Utility Management System

---

## Requirements

### macOS 

- macOS 11+ (Apple Silicon)
- [Homebrew](https://brew.sh/)
- C++17-compatible compiler (e.g., `g++`)
- PostgreSQL client libraries
- `libpqxx`

Install dependencies:

```bash
brew install libpqxx
```

---

### Windows

- Windows 10 or 11
- [vcpkg](https://github.com/microsoft/vcpkg) (C++ package manager)
- C++17-compatible compiler (Visual Studio with MSVC or MinGW)
- PostgreSQL access (Supabase or local)

Install `libpqxx` using vcpkg:

```bash
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
vcpkg install libpqxx
```

---

## Compiling

### macOS

```bash
g++ main.cpp Customer.cpp Bill.cpp \
    -std=c++17 \
    -I/opt/homebrew/include \
    -L/opt/homebrew/lib \
    -lpqxx -lpq \
    -Wno-unknown-attributes -Wno-deprecated-declarations \
    -o utility
```

Run the program:

```bash
./utility
```

---

### Windows

```bash
g++ main.cpp Customer.cpp Bill.cpp ^
    -std=c++17 ^
    -I[path_to_vcpkg]/installed/x64-windows/include ^
    -L[path_to_vcpkg]/installed/x64-windows/lib ^
    -lpqxx -lpq ^
    -o utility.exe
```

Run the program:

```bash
./utility.exe
```

> Replace `[path_to_vcpkg]` with your actual vcpkg path (e.g., `C:/dev/vcpkg`).

