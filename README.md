# Simple-Shell (sish)

`sish` is a basic shell implementation in C. This project provides a simple command-line interface for users to execute built-in commands, system commands, and view command history.

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Built-in Commands](#built-in-commands)
- [Contributing](#contributing)

## Installation

To install and run `sish`, follow the steps below:

```bash
git clone https://github.com/[YOUR_GITHUB_USERNAME]/Simple-Shell.git
cd Simple-Shell
gcc -o sish main.c
./sish
```

## Usage
After launching sish, you'll be presented with a prompt (sish> ). You can start typing commands just like any other shell:
sish> ls
sish> cd ../
sish> history

## Built-in Commands
cd [DIRECTORY]: Change to the specified directory. If no directory is specified, changes to the home directory.
history: Shows the last 10 commands executed.
Use history -c to clear the history.
Use history [INDEX] to execute a command from history.
exit: Exits the shell.

## Contributing
Contributions are welcome! Please fork the repository and create a pull request with your changes.

