# test-repo-2

Contains a random C file to experiment with piping and redirection.
The C file creates 2 child processes, redirects the stdout of both child processes to the write end of a pipe.
One child process prints from 0 to 14 over the course of 15 seconds
The other child process keeps asking for user input until user inputs `stop`.
In the parent process, the read end of the pipe is redirected to the stdin.
The stdin is then written into a file.
