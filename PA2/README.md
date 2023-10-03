[![Build Status](https://travis-ci.org/joemccann/dillinger.svg?branch=master)](https://travis-ci.org/joemccann/dillinger)

## Message queue mechanism

```
When a user launches a program for execution, the OS creates a user process to execute it.

Many OS, including Linux, provides the mechanism for a process to create child processes to run
concurrently with the parent process which creates them. Linux provides such support with system calls fork, exec and wait.

These concurrent processes may need to communicate among them. One way Linux supports interprocess communication is message queue.

Using the System V implementation, a message queue must first be acquired from the operating system by calling msgget. Control operations, e.g., remove, can be performed on an existing message queue by calling msgctl.

Processes with appropriate permissions may send and/or receive messages via the message queue by calling msgsnd and msgrcv.

A process may obtain its own PID by calling getpid. Please consult the man pages of these system calls for details.
```

```sh
127.0.0.1:8000
```


