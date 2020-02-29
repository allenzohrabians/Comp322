Saturday (2/15/2020): 
This is my first README entry for Lab 1. We were just introduced to the project and I initially began my approach by looking up the various 
functions required for the assignment through the C library. This includes time(), fork(), waitpid(), and getpid(). I prefer to do this off 
the bat so I can begin developing ideas on how to implement them before starting to code.

Tuesday (2/18/2020):
This is my second README entry for Lab 1. My main goal of today was getting down the if else statements involving the fork() function set 
up in my main method. In this case we had if the value of the process_id was -1 and an error message would print and the program would 
exit. If the value of the process_id was 0, we'd be within the child process and from there it'll call the parentChildReport() function and 
pass in NULL for both parameters. When the value of the process_id is a postive number, we are then in the parent process which handles the 
printing of the user times and system times for both the parent and the child.
