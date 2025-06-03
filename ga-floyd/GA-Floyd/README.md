# GA_FLOYD
## 人工知能を用いてフロイドの問題を解く

UNIX系の環境下で実行可能

### exp1


### exp2

floyd.cpp ：　実行ファイル  
operate.py ：　floyd.cppの条件を設定し繰り返し実行処理を行う

#### floyd.cpp
詳細はこちらからご確認ください  
[floyd.cppの詳細](./floyd-cpp.md)  

各結果を`result.txt`に出力しています.  
※1回の実行ごとに上書きされます.  

result_list1.csvに作成したデータを追加していく処理があります.  
result_list1の内容を変更したい場合はresult_list1.csvを一度削除し再度実行してファイルの生成をおこなってください.  




基本的に"operate.py"内部の値を変更する
floyd.cppは編集しない


## 環境構築
環境が怪しい場合はdockerを使用してください  
[dockrfileはこちらから](https://github.com/tcu-seki-lab/docker-info/tree/main/ga-floyd)  
`ga-floyd/dockerfile`を使用してください

```sh
# dockerイメージの作成
# docker build -t <イメージ名> <dockerfileのあるPATH>
docker build -t ga-floyd ./

# dockerコンテナの起動
#sudo docker run --name {コンテナ名} -it {image名} /bin/bash
sudo docker run --name ga-floyd-container -it ga-floyd /bin/bash
```

### 環境情報
```
OS : Ubuntu20.04
python version : 3.9.5
gcc version : 9.4.0
```
pythonの実行コマンドは`python3`です  

```sh
# 例 test.pyの実行コマンド
$ python3 test.py
```

UbuntuでのC++実行コマンド
```sh
# コンパイル
g++ floyd.cpp

# 実行
./a.out

```


/exp2/data/doc/  
tmp.py  
ターミナル操作のサンプルプログラム

test.txt
パラメータ初期値

memo.txt  
定数の説明  



analize.py  
作成したcsvの平均をresult-ave.csvとして書き出す処理をおこなっている

処理時間の平均,平方根和の差の平均は8桁表示にしている  


