## コンパイル方法
tehai_group.hppの定数CAL_NUM_THREADをマシンに合わせた適切な数字で置き換える。
Windows環境では、ifdef WINSTDの下の定義が使われ、Linuxだとelse以下の定義が使われるので、利用のOSに合わせて書き換える。
4並列なら4、8並列なら8。よくわからない場合、一旦無視してコンパイルを進めて

./system.exe para_check

と起動してHello Worldが出てきた個数にするとよい。

### system.exeコンパイル手順(linux)
lboost_systemが利用可能か確認。（おそらく使えるはずで、使えない場合aptでlibboost-devをインストールすれば問題無いはず。）  
ai_srcディレクトリで  
$ make -f Makefile_Linux libai.so  
としてAIをビルド。その後  
$ make -f Makefile_Linux system.exe  
とすると全システムのコンパイルができる。
	
### system.exeコンパイル手順(windows)
lboost_systemがリンク可能か確認し、可能であれば Makefile（ai_srcディレクトリとルートディレクトリ両方）のLIBSの定義を書き換える。（boostをビルドすれば可能になるはず。）  
製作者の環境では、"-lboost_system-mgw62-mt-x64-1_70" が利用可能であるが、この部分  
LIBS = -lboost_system-mgw62-mt-x64-1_70  
を自分の環境にあったものに書き換える。  

その後ai_srcディレクトリで  
\> make ai.dll  
としてAIをビルド。続けて  
\> make system.exe  
とすると全システムのコンパイルができる。

## 使用方法

AIを起動するときは、system.exeがあるディレクトリに、paramsディレクトリ、ai.dll(windows)、libai.so(Linux)、setup_mjai.json、setup_match.jsonが必要です。  
牌譜の形式については、
http://critter.sakura.ne.jp/mjai_protocol.html
と付属しているhaifu_log_sample.jsonをご覧ください。

### system.exe test
自動対局を走らせる。対局の結果は、setup_match.jsonのresult_dirで指定されているディレクトリに保存されます。

### system.exe mjai_client
mjaiのクライアントとして起動します。
manueがインストールされている場合、mjai.shを起動させると、このコマンドを経由してmanueとの対局を行います。
インストール方法は
http://critter.sakura.ne.jp/memo_tech/mjai_manue_install.html
をご覧ください。

### system.exe stats dirname
dirnameで指定されているディレクトリにある自動対局牌譜を走査し、各プレイヤの統計値を出力します。

### system.exe stats_mjai dirname
dirnameで指定されているディレクトリにある、manueサーバーで対局した牌譜を走査し、プレイヤの統計値を出力します。

### system.exe mjai_log filename id
filenameで指定された牌譜が対局途中である場合、idのプレイヤの最善手を返します。
東1局の、東、南、西、北家がそれぞれ、0、1、2、3に対応します。例えば  
./system.exe mjai_log haifu_log_sample.json 2  
とすると、サンプル牌譜の局面での最善手が表示されます。

### system.exe para_check
利用しているマシンの並列数をチェックします。

### system.exe initial_condition_match
開始状態を指定して自己対局を行う想定ですが準備中です。

## お問い合わせ
以下で受け付けておりますので、お気軽にお問い合わせください。atmarkは@に変えてください。  
メール:mkmjai1[atmark]gmail.com  
twitter:https://twitter.com/critter_Eng  
質問箱:https://peing.net/ja/critter_eng  
製作者サイト:http://critter.sakura.ne.jp/

