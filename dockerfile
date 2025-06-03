# stage1 : ベースイメージでubuntuを使用．
FROM ubuntu:20.04

# DEBIAN_FRONTEND 環境変数を noninteractive に設定
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y \
    sudo \
    git \
    # タイムゾーン設定に必要なパッケージをインストール
    tzdata\
    vim less\
    make\
    tree

# タイムゾーンを東京に設定
RUN ln -fs /usr/share/zoneinfo/Asia/Tokyo /etc/localtime && dpkg-reconfigure -f noninteractive tzdata

# expect をインストール
RUN apt-get update && apt-get install -y expect

RUN apt-get install -y --no-install-recommends \
    # ファイルのダウンロードツール
    wget \
    # ビルドに必要なツールやライブラリのパッケージ
    build-essential \
    # readline ライブラリの開発用ファイル
    libreadline-dev \
    # ncurses ライブラリの開発用ファイル
    libncursesw5-dev \
    # SSL 関連の開発用ファイル
    libssl-dev \
    # SQLite データベースの開発用ファイル
    libsqlite3-dev \
    # GDBM データベースの開発用ファイル
    libgdbm-dev \
    # bzip2 圧縮の開発用ファイル
    libbz2-dev \
    # lzma 圧縮の開発用ファイル
    liblzma-dev \
    # zlib 圧縮の開発用ファイル
    zlib1g-dev \
    # UUID の開発用ファイル
    uuid-dev \
    # libffi ライブラリの開発用ファイル
    libffi-dev \
    # Berkeley DB データベースの開発用ファイル
    libdb-dev


#python3.9.5 install
RUN wget --no-check-certificate https://www.python.org/ftp/python/3.9.5/Python-3.9.5.tgz \
    && tar -xf Python-3.9.5.tgz \
    && cd Python-3.9.5 \
    && ./configure --enable-optimizations\
    && make \
    && make install

#必要なpythonパッケージをpipでインストール
RUN pip3 install --upgrade pip && pip3 install --no-cache-dir jupyterlab

USER root

RUN apt-get -y install locales && \
    localedef -f UTF-8 -i ja_JP ja_JP.UTF-8

# ENV LANG ja_JP.UTF-8
# ENV LANGUAGE ja_JP:ja
# ENV LC_ALL ja_JP.UTF-8
ENV TZ=JST-9
ENV TERM=xterm
ENV HOME=/usr/local/GA_FLOYD/
WORKDIR $HOME

RUN pip install --upgrade pip
RUN pip install --upgrade setuptools

COPY ./ga-floyd/ $HOME/

WORKDIR $HOME/GA-Floyd



