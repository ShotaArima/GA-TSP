for dataset in berlin52 eil51 eil76 kroA100
do
  for selection in ranking roulette tournament
  do
    for pop in 100 200
    do
      for mut in 0.01 0.05
      do
        ./tsp_ga $dataset $selection pmx $pop $mut
      done
    done
  done
done
