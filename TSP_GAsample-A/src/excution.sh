#!/bin/bash

# データセットの候補
datasets=("berlin52" "eil51" "eil76" "kroA100")

# 選択戦略の候補
selections=("ranking" "roulette" "tournament")

# 交叉法の候補
crossovers=("pmx" "ox" "one_point" "two_point" "uniform")

# 個体数（集団サイズ）
pop_sizes=(100 200)

# 突然変異率
mut_rates=(0.01 0.05)

# 出力ディレクトリの確認
mkdir -p log

# 実行
for dataset in "${datasets[@]}"; do
  for selection in "${selections[@]}"; do
    for crossover in "${crossovers[@]}"; do
      for pop in "${pop_sizes[@]}"; do
        for mut in "${mut_rates[@]}"; do
          echo "Running: ./tsp_ga $dataset $selection $crossover $pop $mut"
          ./tsp_ga "$dataset" "$selection" "$crossover" "$pop" "$mut"
        done
      done
    done
  done
done
