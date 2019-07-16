# Bell_Lapadula_Simulation
Bell Lapadula Access Control model simulation in CPP

## Getting started
There are 3 files included salt,shadow and filesimulation text files to simulate Unix Access Control and file system. 
1. Salt stores the username and the salted file.
2. Password is hashed using md5 implementation by Frank Thilo's (thilo@unix-ag.org) and stored in shadow file.
3. Records of files are stored in file simulation together with security clearance to simulate BLP model.

## To compile (GNU compiler)
```g++ main.cpp md5.cpp -o FileSystem```

## To Initialise and create an account
```./FileSystem -i```

## To login
```./FileSystem```
