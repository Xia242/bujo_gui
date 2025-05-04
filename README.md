# Bullet Journal App (Bujo Projects)

This is a simple Bullet Journal application project designed to provide basic logging, future planning, and review functionalities. The project includes two versions:

1.  **`bujo_gui`**: A Graphical User Interface (GUI) version developed using C++ and the Qt framework.
2.  **`bujo_cmd`**: A Command-Line Interface (CLI) version written in C++.

---

## ✨ Features

### GUI Version (`bujo_gui`)

* **Today View**: Add, edit, delete, and view daily journal entries, including title, content, and tags.
* **Month View**: View the current month in a calendar format and see journal entries for selected dates.
* **Future Log**: Organize and view future plans or events by month.
* **Daily Review**: Record daily reflections and reviews.
* **Data Storage**: Uses JSON format to save all data (journal, future log, reviews).
* **Interface Navigation**: Switch functions and select dates via the left-side navigation panel and calendar.

### CLI Version (`bujo_cmd`)

* **Journal Management**: Add, edit, delete journal entries.
* **Journal Viewing**: Search and view entries by date, title keyword, tags (supports multiple tags with AND logic), or date range.
* **Future Log Management**: Add, edit, delete future log entries, organized by year and month.
* **Monthly Calendar View**: Display a calendar for a specified year and month, marking dates that have journal entries.
* **Data Storage**: Uses a custom plain text format (`journal.txt`) to store journal and future log data.

---

## 🛠️ Technology Stack

* **Language**: C++ (C++17 or higher recommended)
* **GUI Framework**: Qt 5 or Qt 6 (for `bujo_gui`)
* **Build System**: CMake (for `bujo_gui`)

---

## 🚀 Getting Started

### Prerequisites

* **General**:
    * C++ Compiler (e.g., GCC, Clang, MSVC)
* **`bujo_gui` Specific**:
    * CMake (Version 3.16 or higher)
    * Qt Development Libraries (Qt 5.x or Qt 6.x) - Ensure the `Widgets` module is installed.

### Building

#### Building `bujo_gui` (GUI Version)

```bash
# Navigate to the GUI project directory
cd bujo_gui

# Create and enter the build directory
mkdir build
cd build

# Run CMake configuration
# You might need to specify the Qt path if it's not found automatically
# e.g., cmake .. -DCMAKE_PREFIX_PATH=/path/to/your/qt
cmake ..

# Run the build
make
# or use: cmake --build .

#### Building `bujo_cmd` (CLI Version)

```bash
# Navigate to the CLI project directory
cd bujo_cmd

# Compile using a C++ compiler (e.g., g++)
g++ main.cpp -o bujo_cmd -std=c++17

### Running

* **`bujo_gui`**:
    * After building, the executable is typically located in the `bujo_gui/build/` directory.
    * On Linux/macOS, run `./fg` (based on the project name in CMakeLists.txt).
    * On Windows, run `fg.exe`.
* **`bujo_cmd`**:
    * After building, the executable is located in the `bujo_cmd/` directory.
    * On Linux/macOS, run `./bujo_cmd`.
    * On Windows, run `bujo_cmd.exe`.
    * The program will display a menu; follow the prompts to operate.

---

## 💾 Data Storage

* **`bujo_cmd`**: By default, saves data to a `journal.txt` file in the same directory as the executable.
* **`bujo_gui`**: By default, uses the "Open" and "Save" menu options to let the user choose a JSON file for loading and saving data.

**Note**: If you do not want to commit your personal journal content to version control (e.g., Git), it is recommended to add the data files (`journal.txt`, `*.json`) to your `.gitignore` file.

---

## 📄 License

License not specified for this project.

---