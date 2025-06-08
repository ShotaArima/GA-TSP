#!/bin/bash

# 上位20パターンの条件を手動抽出して記述（例）
declare -a configs=(
  "berlin52 tournament ox 100 0.05"
  "berlin52 tournament ox 100 0.01"
  "berlin52 roulette ox 100 0.01"
  "berlin52 roulette ox 100 0.05"
  "berlin52 tournament uniform 100 0.01"
  "eil51 ranking two_point 200 0.01"
  "eil51 ranking two_point 200 0.05"
  "eil51 tournament pmx 200 0.01"
  "eil51 tournament ox 200 0.05"
  "eil51 tournament ox 200 0.01"
  "eil76 ranking one_point 200 0.01"
  "eil76 roulette pmx 100 0.01"
  "eil76 roulette pmx 100 0.05"
  "eil76 tournament two_point 200 0.01"
  "eil76 tournament two_point 200 0.05"
  "kroA100 tournament uniform 100 0.05"
  "kroA100 roulette two_point 200 0.05"
  "kroA100 roulette two_point 200 0.01"
  "kroA100 ranking two_point 100 0.01"
  "kroA100 ranking two_point 100 0.05"
)

mkdir -p log

# 各条件につき10回実行
for cfg in "${configs[@]}"; do
  for i in {1..10}; do
    args=($cfg)
    dataset="${args[0]}"
    selection="${args[1]}"
    crossover="${args[2]}"
    pop="${args[3]}"
    mut="${args[4]}"
    log_file="log/log_${dataset}_${selection}_${crossover}_${pop}_${mut}_run${i}.csv"

    echo "Running: ./tsp_ga $cfg  > $log_file"
    ./tsp_ga $cfg > "$log_file"
  done
done
