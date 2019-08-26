tehai_group.hppの定数CAL_NUM_THREADをマシンに合わせた適切な数字で置き換える。
Windows環境では、ifdef WINSTDの下の定義が使われ、Linuxだとelse以下の定義が使われるので、利用のOSに合わせて書き換える。
4並列なら4、8並列なら8。よくわからない場合、一旦無視してコンパイルを進めて

./system.exe para_check

と起動してHello Worldが出てきた個数にするとよい。

system.exeコンパイル手順(linux)
	lboost_systemが利用可能か確認。（おそらく使えるはずで、使えない場合aptでlibboost-devをインストールすれば問題無いはず。）
	ai_srcディレクトリで
	$ make -f Makefile_Linux libai.so
	としてAIをビルド。その後
	$ make -f Makefile_Linux system.exe
	とすると全システムのコンパイルができる。
	
system.exeコンパイル手順(windows)
	lboost_systemがリンク可能か確認し、可能であればMakefile（ai_srcディレクトリとルートディレクトリ両方）のLIBSの定義を書き換える。（boostをビルドすれば可能になるはず。）
	製作者の環境では、"-lboost_system-mgw62-mt-x64-1_70" が利用可能であるが、この部分
	LIBS = -lboost_system-mgw62-mt-x64-1_70
	を自分の環境にあったものに書き換える。
	
	その後ai_srcディレクトリで
	> make ai.dll
	としてAIをビルド。続けて
	> make system.exe
	とすると全システムのコンパイルができる。
