
# OS Assignment

### Creating a Custom C Shell to Demonstrate the `fork()`, `exec()`, and `wait()` Calls in a Process and Simulate a shell.

This C shell supports:

- **word** *(internal)*: Counts the number of words in a file

  Usage: `word {-n/d} <filenames>`

  - `-n`: Exempt newline character
  - `-d`: Find the difference between two files.

- **dir** *(external)*: Creates a new directory and changes the path into that directory

  Usage: `dir {-r|-v} <Directory>`

  - `-r`: If the directory exists, then delete it recursively and create a new one with the same name
  - `-v`: Print the steps of creation

- **date** *(external)*: Gets a file's latest modification date

  Usage: `date {-R|-d} <filename>`

  - `-R`: Print in RFC5322 format
  - `-d`: Accept STRING inputs like 'yesterday' and 'tomorrow'

Usage: `date -d 'yesterday' <filename>`


*TharunNinja&copy;2023*