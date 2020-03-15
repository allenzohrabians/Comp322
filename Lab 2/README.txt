Monday (3/2/2020):
This is my first README entry for Lab 3. This past Saturday we were introduced to our Launch-Tube assignment. I began immediately by 
modifying the Makefile using the same structure as previous labs, however having separate "ALL" and "REM" entries for the launch program 
and the tube program. From there I began doing research on what I will be needing for launch. We had already worked with fork() and 
waitpid() in Lab 1 so after a quick refresh I proceeded to go over execve() and understand how to implement it within the child process.

Friday (3/6/2020):
This is my second README entry for Lab 3. I've spent the last few days finishing up the launch program. Everything seemed to be working as 
expected. The only trouble I came across was not having the desired output for the parent's printing of the child's return value. Instead 
of the return value being 2 to indicate that it was missing a keyword/command or that there was a permission problem, I had received the 
value of 512. I fixed this through implementing WIFEXITED() and WEXITSTATUS() within the parent process so that we can be sure that the 
child terminated normally and that the correct return value was being displayed.

Friday (3/13/2020):
This is my third README entry for Lab 3. After finishing all my midterms prior to Spring Break, I was able to continue with the project now 
moving on to the tube program. The main thing here that took myself some time to fully grasp was dup2(), even after my initial research on 
it. Conceptually understanding it wasn't an issue, however the actual coding for that function was what was holding me back. After some 
time and assistance from a relative, I was able to get it working within the program.

