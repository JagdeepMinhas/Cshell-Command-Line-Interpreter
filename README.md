

```markdown
# CShell - Simple Command-Line Interpreter

CShell is a basic command-line interpreter that supports environment variables, built-in commands, and execution of external programs.

## Features

- **Interactive Mode:** Start the shell without command line arguments to enter interactive mode. In this mode, you can execute commands and use built-in functions.

- **Script Mode:** Run the shell with a script file as a command-line argument to execute a series of commands from the script.

- **Environment Variables:** Store and retrieve environment variables in the form of `$VAR=value`. These variables can be used within commands.

- **Built-in Commands:**
  - `exit`: Terminate the shell.
  - `print`: Print arguments provided to the command.
  - `theme`: Change the output text color (supports red, green, and blue).
  - `log`: Display a history of executed commands with timestamps and return codes.

## Usage

### Interactive Mode

To run the shell in interactive mode, simply execute the `cshell` binary without any command-line arguments:

```bash
./cshell
```

You will be prompted with `cshell$` to enter commands. Use the built-in commands or execute external programs.

### Script Mode

To run the shell in script mode, provide a script file as a command-line argument:

```bash
./cshell script.txt
```

The script file should contain a series of commands to be executed. The shell will execute each command sequentially.

## Environment Variables

You can define environment variables using the following syntax:

```bash
$VAR=value
```

To use an environment variable in a command, simply prefix it with a `$` symbol:

```bash
echo $VAR
```

## Examples

Here are some example commands and their expected outputs:

```bash
$VAR1=Hello
print $VAR1
```

Output:
```
Hello
```

```bash
theme blue
print This is blue text.
theme red
print This is red text.
```

Output:
```
This is blue text. (in blue)
This is red text. (in red)
```

```bash
log
```

Output:
```
1. Command: print Hello World, Time: 2023-09-01 12:34:56, Return Code: 0
2. Command: ls, Time: 2023-09-01 12:35:10, Return Code: 0
```

## Error Handling

- If a command or input is invalid, the shell will display an error message.
- For unknown commands or unsupported themes, an error message will be shown.

## Notes

- This shell is a basic implementation and may not cover all edge cases or advanced features of a typical shell.
- Use the `exit` command to terminate the shell cleanly.

## Author

[Jagdeep Singh]

Feel free to customize and expand this `README.md` file to include any additional information or instructions specific to your assignment.
