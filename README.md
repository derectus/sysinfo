# SysInfo

System call implemtation to linux kernel. This project includes kernel space and User space. The aim of the project is to get some specific files from the **/proc** file. Some scenarios are described below.


# Implementation Details:
The project running int the user space run the system call is named as “systeminfo”. Project takes two parameters as **-p** and **-all**. Firstly, it prints information about CPU such as processor, vendor id, CPU family, model name, CPU MHz.


## Additionally, it may be taken as same or different time optionally the following parameters:

 - “**-p processID**”, it prints some details about the given process ID such as user, pid,
start_time, command.
 - “**-all**”, it prints the length of time since the system was booted, as well as the amount of
time since then that the system has been idle, the number of active tasks on the system and
and the total number of processes on the system.

## The sample executing formats:
./systeminfo.out > only CPU information

./systeminfo.out -all > CPU+system statistics

./systeminfo.out -p 1425 > CPU+process detail

./systeminfo.out -all -p 1425 > CPU+sys.statistics+ps.detail

## The sample output should be like that, if two parameters were used:
![sample](screenshot)
