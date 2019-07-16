# Bell_Lapadula_Simulation
Bell Lapadula Access Control model simulation in CPP

## Getting started
There are 2 files included salt and shadow to simulate Unix Access Control system. Salt stores the username and the salted file.
Password is hashed using md5 implementation by Frank Thilo's (thilo@unix-ag.org)

## To compile (GNU compiler)
```g++ main.cpp md5.cpp -o FileSystem```

## To Initialise and create an account
```./FileSystem -i```

## To login
```./FileSystem```
