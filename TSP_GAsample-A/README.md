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

## 引数のパターンについて
`./tsp_ga [dataset] [selection] [crossover] [popSize] [mutateRate]`

引数	内容	例
dataset	TSPインスタンスの名前（例：berlin52）	berlin52
selection	選択戦略の種類（ranking, roulette, tournament）	ranking
crossover	交叉法の種類（今は pmx のみ対応）	pmx
popSize	個体数（集団サイズ）	100
mutateRate	突然変異率（例：0.01で1%）	0.01

### 例
```bash
./tsp_ga berlin52 ranking pmx 100 0.01
```

- データセット: berlin52
- 選択方式: ranking（ランキング選択）
- 交叉方式: pmx（部分写像交叉）
- 個体数: 100
- 突然変異率: 1%


## パラメータについて
### 1. データセット

|候補|備考|
|---|----|
|berlin52|ベルリン市内52地点|
|eil51|51地点のTSP|
|eil76|76地点のTSP|
|kroA100|100地点（Krolikowski）|
|test|自作小規模データ（TSP_data/test.tspが必要）|

### 選択方式
|候補|備考|
|---|----|
|ranking|ランキング選択（順位に比例して選ばれる）|
|roulette|ルーレット選択（適応度に比例して選ばれる）|
|tournament|トーナメント選択（複数から勝者を選ぶ）|

### 交叉方式

|候補|備考|
|---|----|
|`pmx`|	部分写像交叉（Partially Mapped Crossover）|
|（今後追加可能）|ox（順序交叉）なども追加可能|

### 個体数

|候補|備考|
|---|----|
|50|	小規模検証向け|
|100|	標準的な設定|
|100|	安定性重視（実行時間増加）|
|500|	精度重視・時間要注意|


### 突然変異率

|候補例|説明|
|---|----|
|0.001|非常に低い（探索範囲狭い）|
|0.01|標準的|
|0.05|探索重視（多様性高）|
|0.1|高頻度（暴走リスクあり）|