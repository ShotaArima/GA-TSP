# Aコース

## 起動・実行方法
```bash
docker compose up -d　# 起動
docker compose exec app bash # コンテナに侵入
g++ *.cpp -o tsp_ga # コンパイル
./tsp_ga  # 実行
```

## 構成
```bash
.
├── compose.yaml
├── dockerfile
├── README.md
└── src/
    ├── Field.cpp / Field.h         : 入力ファイル読み込み・距離計算
    ├── Individual.cpp / .h        : 染色体構造・評価・突然変異など
    ├── Population.cpp / .h        : 世代交代、選択、交叉、評価の管理
    ├── TSP_GA.cpp                 : 実行の起点、データ選択UI
    ├── TSP_data/                  : TSPLIBから取得したデータ
    └── tsp_ga/                    : バイナリファイル
```