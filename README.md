# Utility Management System

---

## Requirements

### macOS

- macOS 11+
- [Homebrew](https://brew.sh/)
- `g++` or `clang++` with C++17 support
- Qt 6
- PostgreSQL client libraries
- `libpqxx` (C++ PostgreSQL wrapper)

#### Install dependencies:

```bash
brew install qt libpqxx
```

---

### Windows

- Windows 10 or 11
- [vcpkg](https://github.com/microsoft/vcpkg) (C++ package manager)
- C++17-compatible compiler (Visual Studio with MSVC or MinGW)
- PostgreSQL access (Supabase or local)
- Qt 6

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
g++ -std=c++17 -fPIC -Wno-unknown-attributes -Wno-deprecated-declarations \
    main.cpp MainWindow.cpp CustomerDialog.cpp ProviderDialog.cpp InvoiceDialog.cpp ServiceDialog.cpp \
    moc_MainWindow.cpp moc_CustomerDialog.cpp moc_ProviderDialog.cpp moc_InvoiceDialog.cpp moc_ServiceDialog.cpp \
    Customer.cpp Provider.cpp Service.cpp Invoice.cpp Bill.cpp \
    -I/opt/homebrew/include \
    -I/opt/homebrew/opt/libpqxx/include \
    -I/opt/homebrew/opt/libpq/include \
    -L/opt/homebrew/lib \
    -L/opt/homebrew/opt/libpqxx/lib \
    -L/opt/homebrew/opt/libpq/lib \
    $(pkg-config --cflags --libs Qt6Widgets) \
    -lpqxx -lpq \
    -o utility_gui




```

Run the program:

```bash
./utility_gui
```

---

### Windows

```bash
g++ -std=c++17 -fPIC ^
    main.cpp MainWindow.cpp moc_MainWindow.cpp ^
    Customer.cpp Provider.cpp Service.cpp Invoice.cpp Bill.cpp ^
    -I[path_to_vcpkg]/installed/x64-windows/include ^
    -L[path_to_vcpkg]/installed/x64-windows/lib ^
    -lpqxx -lpq ^
    -o utility_gui.exe

```

Run the program:

```bash
./utility_gui.exe

```

> Replace `[path_to_vcpkg]` with your actual vcpkg path (e.g., `C:/dev/vcpkg`).

