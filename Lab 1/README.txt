Tuesday (2/18/2020): 
This is my first README entry for Lab 1. We were just introduced to the project and I initially began my approach by looking up the various 
functions required for the assignment through the C library. This includes time(), fork(), waitpid(), and getpid(). I prefer to do this off 
the bat so I can begin developing ideas on how to implement them before starting to code.

Saturday (2/22/2020):
This is my second README entry for Lab 1. My main goal of today was getting down the if else statements involving the fork() function set 
up in my main method. In this case we had if the value of the process ID was -1 and an error message would print and the program would 
exit. If the value of the process ID was 0, we'd be within the child process and from there it'll call the parentChildReport() function and 
pass in NULL for both parameters. When the value of the process ID is a postive number, we are then in the parent process which handles the 
printing of the user times and system times for both the parent and the child.

Monday (2/24/2020):
This is my third README entry for Lab 1. Continuing with the project we move on towards the parentChildReport() function. Its paramters 
include pointers to both the process ID as well as the status location. Within the process I made calls to retrieve the pid and ppid. From 
there I created an if else statement in which if the values for the process ID as well as the status location were set to NULL (indicating 
it is coming from the child process) then we would only print the PID and PPID. In the case of else (indicating it is coming from the 
parent process), alongside printing out the PID and PPID we would also print out the PID of the child and the return status of the child.
