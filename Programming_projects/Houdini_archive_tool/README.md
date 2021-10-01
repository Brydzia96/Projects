# HOUDINI ARCHIVE TOOL

It is a bash script that allows for a quick archive (tar format) of a directory containing Houdini scene files (Houdini project) along with its subdirectories (e.g. sim, render, tex etc).

## NAME
houdini_tool.sh - an archiving utility for Houdini projects.

## SYNOPSIS

```bash
sh houdini_tool.sh -s [PATH...] -t [FILE...] [OPTIONS]
```
## DESCRIPTION
The script is using four flags which allow to pass one mandatory and three optional inputs:

-s: (it stands for "source") it takes the source path for the directory to be archived (or a name of the directory if it is located in the current directory), it is mandatory to run the script,

-t (it stands for "target") it takes the target path with the target name (with .tar extension),

-f (it stands for "file"):
if the flag is used, the summary of a archived directory in json format will be put in the source and will be archived along with it,

-r (it stands for "remove"):
if the flag is used, the source directory is removed.

## SCRIPT
The script is divided in five main parts:
1. The getopts function, 
2. Initial source and target check,
3. Declaration of functions used to check if the provided directory is a valid Houdini Project,
4. JSON file creation,
5. Execution.<br/>

Ad 1. The script starts with declaration of the variables used in a getopts function. By default "summary" and "remove" options are optional therefore their initial value is a boolean "false". The getopts function reads the flags in the input and ${OPTARG} refers to the corrensponding values.

Ad
2. The -s input provided by the user is checked:<br/>
a) if there is no argument the script exits and prints the message: "Source not provided",<br/>
b) if the directory exists the source provided becomes the source directory (the directory to be archived),<br/>
c) otherwise the script exits and prints the message "Wrong source provided".<br/>
Subsequently, the -t input provided by the user is checked:<br/>
a) if the flag is not used, the archive file will be placed in the directory that contained the source to be archived and will be named after source,<br/>
b) in the case only a name is used the location of a target will be in the directory that contained the source,<br/>
c) if the path provided does not exists or the name is invalid the script exits and prints the message: "Wrong target provided".<br/>

Ad
3. Functions:<br/>
a) correct_directory function checks if the source directory contains all the default directories of Houdini project which are: abc, audio, comp, desk, flip, geo, hda, render, scripts, sim, tex and video. It takes a path to the project as an argument and pushes the names of its subdirectories to the string. Later on, it compares the strings and if they are not equal it returns the message to the user "Project directories are not complete.Continue? <Enter yes if so>". It gives the option to exit the script in this moment. The user might input "yes" to continue archiving despite the unusual hierarchy of the Houdini project directories.<br/>
b) file_exist function checks if the source directory contains any hipnc or hip files. It exits the script if not and returns the message: "The main directory does not contain any Houdini files".<br/>
  
Ad
4. The JSON file creation part collects the data about the source directory which are: the project name, path, and size. It also collects the name, modification date and file size of Houdini files and subsequently the name of each subdirectory with its files (the file name, modification date, type and size). In the end it also collects the date when the archiving script was run. 
 
Ad
5. The execution part is divided into three parts:<br/>
a) Make summary: if the -f flag was used the script saves the collected data about the project into "Summary_raw.json file in the source directory. It runs the jq command on a saved file and redirects it to the new file Summary.json (to make json code more readable). The old raw file is removed. It prints the message "Summary created",<br/>
b) Make archive: correct_directory and file_exist functions are called and archive is executed with tar command. It prints the message "Archive done",<br/>
c) Remove: if the -r flag is used the script removes the original source project. It prints the message "Original directory removed".<br/> 
