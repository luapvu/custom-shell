

1. Basic Shell Setup:
1.1 To begin, compile the src files by navigating to the /root/src directory and typing "make1."
Confirm it compiled correctly, your output should look like this:
gcc main.o -o my_shell

1.2. Run the executable by running ./my_shell
The following commands have been tested and are supported:
Internal commands:
pwd, history, cd, exit

External commands:
gedit, atom, mkdir, touch,



2. Internal Commands:
2.1 cd internal command
sample input:
mkdir test_cd
cd test_cd
mkdir test_cd2
cd test_cd2
cd ../../
cd test_cd/test_cd2

Ex:
-------Paul's Terminal-------: mkdir test_cd
-------Paul's Terminal-------: cd test_cd
-------Paul's Terminal-------: /home/pvu/Desktop/feb4/root/src/test_cd
-------Paul's Terminal-------: mkdir test_cd2
-------Paul's Terminal-------: cd test_cd2
-------Paul's Terminal-------: /home/pvu/Desktop/feb4/root/src/test_cd/test_cd2
-------Paul's Terminal-------: cd ../../
-------Paul's Terminal-------: /home/pvu/Desktop/feb4/root/src
-------Paul's Terminal-------: cd test_cd/test_cd2
-------Paul's Terminal-------: /home/pvu/Desktop/feb4/root/src/test_cd/test_cd2



2.2 exit internal command
At anytime type "exit" to exit out of the program

2.3 pwd internal command
At anytime type "pwd" to find your current working directory

Ex:
-------Paul's Terminal-------: pwd
-------Paul's Terminal-------: /home/pvu/Desktop/feb4/root/src

2.4 history internal command
At anytime type "history" to print out up to the last 10 commands.

Ex:
-------Paul's Terminal-------: history
 [5]: cd ../../

 [6]: cd test_cd/test_cd2

 [7]: history

 [8]: pwd

 [9]: atom file1 file2

 [10]: history

 [11]: pwd

 [12]: cd ..

 [13]: cd ..

 [14]: history

-------Paul's Terminal-------:

3. External commands
The following commands have been tested and are confirmed to be supported:

3.1 Open up atom with two files called file1 and file2
atom file1 file2

3.2 Make a new folder
mkdir new_folder

3.3 Make a new file
touch TESTFILE.md

3.4 Open up gedit with 3 files called f1 f2 f3
gedit f1 f2 f3
