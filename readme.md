## Preparation for Building

Replace CAL_NUM_THREAD in tehai_group.hpp with appropriate number of using CPU.
Definition under "ifdef WINSTD" is used for Windows machine, and definition under "else" is used for Linux machine.
Set it to 4 if the CPU is 4 parallel, and set it to 8 if the CPU is 8 parallel.
If you don't understand, skip this and build it then check with the following command.

```./system.exe para_check```

The number you see "Hello World" is the number you should set to CAL_NUM_THREAD.  

## Build with Linux
Check you can use lboost_system.
If not, probably you can install libboost-dev with apt.
Run following in "ai_src" directory.

```$ make -f Makefile_Linux```

You will see libai.so in parent directory.
Then run following in root directory

```$ make -f Makefile_Linux```

Then you will see system.exe in root directory.  

## Build with Windows
Check you can link lboost_system, and rewrite LIBS of Makefile(both in ai_src and root directory).
In the environment of the author, "-lboost_system-mgw62-mt-x64-1_70" is valid.
Line should be rewriten is following.

```LIBS = -lboost_system-mgw62-mt-x64-1_70```  

Then run following in "ai_src" directory.

```> make```

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

