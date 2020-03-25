## system.exeコンパイル手順(linux)
lboost_systemが利用可能か確認。以下のコマンドで利用可能となるはず。

```$ sudo apt-get install libboost-all-dev``` 

ai_srcディレクトリで  

```$ make -f Makefile_Linux```

としてAIをビルド。その後  

```$ make -f Makefile_Linux```  

とすると全システムのコンパイルができる。  

## system.exeコンパイル手順(windows)
lboost_systemがリンク可能か確認し、可能であればMakefile（ai_srcディレクトリとルートディレクトリ両方）のLIBSの定義を書き換える。（boostをビルドすれば可能になるはず。）
製作者の環境では、"-lboost_system-mgw62-mt-x64-1_70" が利用可能であるが自分の環境にあったものに書き換える。 

ai_src/MakefileのNPROCSにCPUの論理コア数を代入する。
CPUの論理コア数は以下のコマンドで確認可能

```> $cs = Get-WmiObject -class Win32_ComputerSystem; $cs.numberoflogicalprocessors```

その後ai_srcディレクトリで  

```> make```

としてAIをビルド。

続けてルートディレクトリで

```> make```  

とすると全システムのコンパイルができる。  
