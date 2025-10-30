# MP3 Tag Reader and Editor

## 📘 Overview

This project is a command-line based **MP3 Tag Reader and Editor** written in C. 

It allows users to view and edit metadata (ID3 tags) from MP3 files such as **Title, Artist, Album, Year, Comment, Genre, and Track Number**. 
The tool provides an efficient way to manage song information without needing a GUI-based tag editor.

---

## ⚙️ Features

* View ID3v2 (v2.3 and 2.4) metadata from MP3 files.
* View or modify individual tags such as Title, Artist, Album, Year, Comments, Track, and Genre.
* Simple and modular command-line interface.
* Display detailed help instructions for usage
* Modular structure with separate source files for reading, writing, and utility operations
* Input validation for input file, supported flags, and argument combinations.
* Error handling for invalid inputs or unsupported commands.

---

## 🧩 Project Structure

```
MP3_Tag_Reader_Editor/
|
├── .gitignore
├── edit.c              # Handles editing of tag frames
├── main.c              # Entry point, handles argument parsing and operation control
├── mp3_tag_reader      # Compiled executable
├── mp3.h               # Header file with structure definitions and function declarations
├── operations.c        # Validates CLI arguments and performs requested operations
├── read.c              # Reads ID3 tag frames from the MP3 file
├── utils.c             # Contains helper functions
├── view.c              # Displays tag details in formatted output
├── write.c             # Handles writing updated tags back to the file
```

---

## 🧱 Compilation

To compile the project:

```bash
gcc *.c -o mp3_tag_reader
```

This will generate the executable file `mp3_tag_reader`.

---

## 🧠 Usage

### Display Help Menu

```bash
./mp3_tag_reader -h
```

**Output:**

```
Help menu for Mp3 Tag viewer and editor:

For viewing the tags :  ./mp3_tag_reader -v <file_name.mp3>
For editing the tags :  ./mp3_tag_reader -e <file_name.mp3> modifiers <tag>

Modifier   Function
   -t      Modifies Title tag
   -T      Modifies Track tag
   -a      Modifies Artist tag
   -A      Modifies Album tag
   -y      Modifies Year tag
   -c      Modifies Comments tag
   -g      Modifies Genre tag
```

### View MP3 Tag Details

```bash
./mp3_tag_reader -v Liar.mp3
```

**Output Example:**

```
MP3 Tag Reader :
----------------------------------------
Version    : ID3 V2.3.0
Title      : Liar
Album      : Romance
Year       : 2018
Track      : hello
Genre      : Dance Pop
Artist     : Camila Cabello
Comments   : This is an edited comment!
----------------------------------------
```

### Edit Specific Tag

```bash
./mp3_tag_reader -e Liar.mp3 -T 1
```

**Output Example:**

```
MP3 Tag Reader :
----------------------------------------
Version    : ID3 V2.3.0
Title      : Liar
Album      : Romance
Year       : 2018
Track      : 1
Genre      : Dance Pop
Artist     : Camila Cabello
Comments   : This is an edited comment!
----------------------------------------
Track has been modified!
```

---

## 🧩 Core Logic

* **`arguments_check()`** (in `operations.c`): Validates command-line arguments and ensures correct usage patterns.
* **`do_operation()`** (in `operations.c`): Handles the appropriate read or edit action based on user input.
* **`edit_frame()` and `add_frame()`**: Modify or insert new tag frames into the metadata.
* **`print_frames()`**: Nicely formats and displays metadata to the user.

---

## 🧠 Concepts Used

* FILE I/O operations
* Command-line argument parsing
* Structure and buffer operations
* Strings and memory manipulation
* Modular C programming
* ID3v2 tag format parsing

---

## 🚧 Future Improvements

* Add support for other versions of **ID3** (v1.0, v1.0, v2.1 and v2.2).
* Provide a feature to **delete** specific tags.
* Implement **batch processing** for multiple MP3 files.
* Optional **GUI** Interface.
* Cross-platform file path handling for Windows/Linux compatibility.

---

## 🧑‍💻 Author

**Alvin J**

[GitHub: A1vin-J](https://github.com/A1vin-J)


---

## 📄 License

This project is open-source and available under the **MIT License**.
