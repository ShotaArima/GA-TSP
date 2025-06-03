# GA-FLOYD docker build方法

1. docker 起動  
2. このディレクトリにて以下のコマンドを入力  
```sh
#terminal or Gitbash

# dockerfileを実行
docker build -t ga-floyd ./

# コンテナの起動
docker run --name ga-floyd-container -it ga-floyd /bin/bash
```

以下dockerで起動したUbuntu

```sh
# 最初のPATH
root@1d20f5cb3ba3:/usr/local/GA_FLOYD/ga-floyd/GA-Floyd# ls
README.md  analize.py  floyd-cpp.md  floyd.cpp  operate.py

# Python 実行コマンドは pytho3
root@e5dc10c38f6a:/usr/local/GA_FLOYD/GA-Floyd# python3 operate.py 

```

## ディレクトリ構造
以下はホームディレクトリからの構造である
```
`-- GA-Floyd
    |-- README.md
    |-- analize.py
    |-- floyd-cpp.md
    |-- floyd.cpp
    `-- operate.py
```