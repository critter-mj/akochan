## Build with Linux
Check you can use lboost_system.
If not, probably you can install it with

```$ sudo apt-get install libboost-all-dev```

Run following in "ai_src" directory.

```$ make -f Makefile_Linux```

You will see libai.so in root directory.

Then run following in root directory

```$ make -f Makefile_Linux```

Then you will see system.exe in root directory.  

## Build with Windows
Check you can link lboost_system, and rewrite LIBS of Makefile(both in ai_src and root directory).
In the environment of the author, "-lboost_system-mgw62-mt-x64-1_70" is valid.

Check the number of logical processors of your machine and rewrite NPROCS value of ai_src/Makefile.
You can check the number by

```> $cs = Get-WmiObject -class Win32_ComputerSystem; $cs.numberoflogicalprocessors```

Then run following in "ai_src" directory.

```> make```

You will see ai.dll in root directory.

Next, run following in root directory.

```> make```

Then you will see system.exe in root directory.

## Selfmatch
Selfmatch will be executed by following command

```./system.exe test "begin(int)" "end(int)"```

Self-matches will be executed with random seed of begin to end - 1.
Game record will be saved in a directory specified by "result_dir" of setup_match.json.
Chicha of self-matches are those specified in "chicha" of setup_match.json.
For example, when begin=100, end=102 and "chicha" is [0, 1], following 4 mathces will be executed.

random seed = 100, chicha = 0  
random seed = 100, chicha = 1  
random seed = 101, chicha = 0  
random seed = 101, chicha = 1  

This is efficient when you want to update AI strategy and play against previous version.
Tactics of each player can be set with "tactics" of setup_match.json.

