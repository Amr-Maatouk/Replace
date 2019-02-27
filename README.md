# Replace

C application for replacing strings within a file.

## Getting Started

Download Project files to your local machine.

```
 $ git clone https://github.com/Amr-Maatouk/Replace
```


### Compilling the project

Using gcc Compiler.

Navigate to project folder.

Compile project "main.c" file using the following command

```
 $ gcc -o  replace main.c
```

## Running the utility.

Using Command line.

Navigate to project folder.

```
 $ replace OPT <from> <to> -- <filename> [<filename>]*
```
where OPT can be zero or more of

○ -b

○ -f

○ -l

○ -i

**COMMAND-LINE ARGUMENTS AND OPTIONS**

from: string to be replaced with string to.

to: string that will replace string from.

filename: the file(s) on which the replace operation has to be performed.

-b: if specified, the replace utility creates a backup copy of each file on
which a replace operation is performed before modifying it.

-f: if specified, the replace utility only replaces the first occurrence of string
from in each file.

-l: if specified, the replace utility only replaces the last occurrence of string
from in each file.

-i: if specified, the replace utility performs the search for string from in a
case insensitive way.


## Load project to Code::Blocks IDE

NOTE: you need to have Code::Blocks IDE and gcc compiler installed on you local machine.

1- open Code::Blocks IDE.

2- File -> Open.

3- Navigate to project folder and choose project's file "Replace.cbp".


## Authors

* **Amr Maatouk**



