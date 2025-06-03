import subprocess #terminalコマンド操作様のライブラリ
import time    #sleepを使うため



'''
設定箇所：以下の配列部分の変更を行うことで自動的にfloyd.cppの実行を行う
num で　全体の繰り返し回数を設定

配列の[0]の部分とfloyd.cppのDEFINEの値を同じにしてから実行をかける
※floyd.cppの方は基本的に数値は変更しない
・以下の#で囲まれたところの値を変更する

// 定数の定義
・GENERATION_MAX			:世代交代数
・POP_SIZE				:個体集団内の個体の数
・ELITE					:エリート保存戦略で残す個体の数
・TOURNAMENT_SIZE		:トーナメント選択で無作為抽出する個体の数
・MUTATE_PROB			:突然変異確率
・MAX_NUM				:フロイド問題におけるN
・FUNCTION1				:選択方式の関数呼び出し
・FUNCTION1EXPORT		:選択方式の名称
・FUNCTION2				:交叉の関数呼び出し
・FUNCTION2EXPORT		:交叉の名称


以下はあまり設定しない様に!!!
------------------------------------------------------
func1 = ["rankingSelect()","tournamentSelect()"]
func2 = ["crossover1","crossover2","crossoverU"]
func1ex = ["\"ランキング選択\"","\"トーナメント選択\""]
func2ex = ["\"1点交叉\"","\"2点交叉\"","\"一様交叉\""]
------------------------------------------------------

追記
numのfor文のカウント用変数はミスったのでhになってる

'''


######################################################
gene_max = [100] #繰り返したいgene_maxの値を入力
pop_size = [100]
elite = [1]
tounament_size = [10]
mutate_prob = [0.01]
max_num = [64]
func1 = ["rankingSelect()","tournamentSelect()"]
func2 = ["crossover1","crossover2","crossoverU"]
func1ex = ["\"ランキング選択\"","\"トーナメント選択\""]
func2ex = ["\"1点交叉\"","\"2点交叉\"","\"一様交叉\""]
num = 10 #全体の繰り返し回数
######################################################


def replace_func(fname, replace_set):
	target, replce = replace_set

	with open(fname,'r') as f1:
		tmp_list = []
		for row in f1:
			if row.find(target) != -1:
				tmp_list.append(replce)
			else:
				tmp_list.append(row)
	with open(fname,'w') as f2:
		for i in range(len(tmp_list)):
			f2.write(tmp_list[i])



# floyd.cppに初期値代入

fname = './floyd.cpp'
replace_setA = ('#define GENERATION_MAX',"#define GENERATION_MAX	"+str(gene_max[0])+"\n")
replace_setB = ('#define POP_SIZE',"#define POP_SIZE	" + str(pop_size[0])+"\n")
replace_setC = ('#define ELITE',"#define ELITE	" + str(elite[0])+"\n")
replace_setD = ('#define TOURNAMENT_SIZE',"#define TOURNAMENT_SIZE	" + str(tounament_size[0])+"\n")
replace_setE = ('#define MUTATE_PROB',"#define MUTATE_PROB	" + str(mutate_prob[0])+"\n")
replace_setF = ('#define MAX_NUM',"#define MAX_NUM	" + str(max_num[0])+"\n")
replace_setG = ('#define FUNCTION1',"#define FUNCTION1	" + str(func1[0])+"\n")
replace_setH = ('#define FUNC1EXPORT',"#define FUNC1EXPORT	" + str(func1ex[0])+"\n")
replace_setI = ('#define FUNCTION2',"#define FUNCTION2	" + str(func2[0])+"\n")
replace_setJ = ('#define FUNC2EXPORT',"#define FUNC2EXPORT	" + str(func2ex[0])+"\n")


replace_func(fname , replace_setA)
replace_func(fname , replace_setB)
replace_func(fname , replace_setC)
replace_func(fname , replace_setD)
replace_func(fname , replace_setE)
replace_func(fname , replace_setF)
replace_func(fname , replace_setG)
replace_func(fname , replace_setH)
replace_func(fname , replace_setI)
replace_func(fname , replace_setJ)



for h in range (num):
	for i in range(len(gene_max)):
		file_name = "floyd.cpp"
		with open(file_name, encoding="UTF-8")as f:
			data_lines = f.read()

		if i == len(gene_max)-1:
			data_lines = data_lines.replace("GENERATION_MAX	" + str(gene_max[i]),"GENERATION_MAX	" + str(gene_max[i-len(gene_max)+1]))
		else:
			data_lines = data_lines.replace("GENERATION_MAX	" + str(gene_max[i]),"GENERATION_MAX	" + str(gene_max[i+1]))
		with open(file_name, mode = "w", encoding="UTF-8")as f:
			f.write(data_lines)
			for j in range(len(pop_size)):

				file_name = "floyd.cpp"
				with open(file_name, encoding="UTF-8")as f:
					data_lines = f.read()
				if j == len(pop_size)-1:
					data_lines = data_lines.replace("POP_SIZE	" + str(pop_size[j]),"POP_SIZE	" + str(pop_size[j-len(pop_size)+1]))
				else:
					data_lines = data_lines.replace("POP_SIZE	" + str(pop_size[j]),"POP_SIZE	" + str(pop_size[j+1]))
				with open(file_name, mode = "w", encoding="UTF-8")as f:
					f.write(data_lines)
				for k in range(len(elite)):
					file_name = "floyd.cpp"
					with open(file_name, encoding="UTF-8")as f:
						data_lines = f.read()
					if k == len(elite)-1:
						data_lines = data_lines.replace("ELITE	" + str(elite[k]),"ELITE	" + str(elite[k-len(elite)+1]))
					else:
						data_lines = data_lines.replace("ELITE	" + str(elite[k]),"ELITE	" + str(elite[k+1]))
					with open(file_name, mode = "w", encoding="UTF-8")as f:
						f.write(data_lines)

					for l in range(len(tounament_size)):
						file_name = "floyd.cpp"
						with open(file_name, encoding="UTF-8")as f:
							data_lines = f.read()
						if l == len(tounament_size)-1:
							data_lines = data_lines.replace("TOURNAMENT_SIZE	" + str(tounament_size[l]),"TOURNAMENT_SIZE	" + str(tounament_size[l-len(tounament_size)+1]))
						else:
							data_lines = data_lines.replace("TOURNAMENT_SIZE	" + str(tounament_size[l]),"TOURNAMENT_SIZE	" + str(tounament_size[l+1]))
						with open(file_name, mode = "w", encoding="UTF-8")as f:
							f.write(data_lines)

						for m in range(len(mutate_prob)):
							file_name = "floyd.cpp"
							with open(file_name, encoding="UTF-8")as f:
								data_lines = f.read()
							if m == len(mutate_prob)-1:
								data_lines = data_lines.replace("MUTATE_PROB	" + str(mutate_prob[m]),"MUTATE_PROB	" + str(mutate_prob[m-len(mutate_prob)+1]))
							else:
								data_lines = data_lines.replace("MUTATE_PROB	" + str(mutate_prob[m]),"MUTATE_PROB	" + str(mutate_prob[m+1]))
							with open(file_name, mode = "w", encoding="UTF-8")as f:
								f.write(data_lines)

							for n in range(len(max_num)):
								file_name = "floyd.cpp"
								with open(file_name, encoding="UTF-8")as f:
									data_lines = f.read()
								if n == len(max_num)-1:
									data_lines = data_lines.replace("MAX_NUM	" + str(max_num[n]),"MAX_NUM	" + str(max_num[n-len(max_num)+1]))
								else:
									data_lines = data_lines.replace("MAX_NUM	" + str(max_num[n]),"MAX_NUM	" + str(max_num[n+1]))
								with open(file_name, mode = "w", encoding="UTF-8")as f:
									f.write(data_lines)








								for o in range(len(func1)):
									file_name = "floyd.cpp"
									with open(file_name, encoding="UTF-8")as f:
										data_lines = f.read()
										#data_lines2 = f.read()
									if o == len(func1)-1:
										data_lines = data_lines.replace("FUNCTION1	" + str(func1[o]),"FUNCTION1	" + str(func1[o-len(func1)+1]))
										# data_lines = data_lines.replace("FUNC1EXPORT	" + str(func1ex[o]),"FUNC1EXPORT	" + str(func1ex[o-1]))
									else:
										data_lines = data_lines.replace("FUNCTION1	" + str(func1[o]),"FUNCTION1	" + str(func1[o+1]))
										# data_lines = data_lines.replace("FUNC1EXPORT	" + str(func1ex[o]),"FUNC1EXPORT	" + str(func1ex[o+1]))
									with open(file_name, mode = "w", encoding="UTF-8")as f:
										f.write(data_lines)

									file_name = "floyd.cpp"
									with open(file_name, encoding="UTF-8")as f:
										data_lines = f.read()
										#data_lines2 = f.read()
									if o == len(func1)-1:
										#data_lines = data_lines.replace("FUNCTION1	" + str(func1[o]),"FUNCTION1	" + str(func1[o-1]))
										data_lines = data_lines.replace("FUNC1EXPORT	" + str(func1ex[o]),"FUNC1EXPORT	" + str(func1ex[o-len(func1ex)+1]))
									else:
										# data_lines = data_lines.replace("FUNCTION1	" + str(func1[o]),"FUNCTION1	" + str(func1[o+1]))
										data_lines = data_lines.replace("FUNC1EXPORT	" + str(func1ex[o]),"FUNC1EXPORT	" + str(func1ex[o+1]))
									with open(file_name, mode = "w", encoding="UTF-8")as f:
										f.write(data_lines)



									for p in range(len(func2)):
										file_name = "floyd.cpp"
										with open(file_name, encoding="UTF-8")as f:
											data_lines = f.read()
											#data_lines2 = f.read()
										if p == len(func2)-1:
											data_lines = data_lines.replace("FUNCTION2	" + str(func2[p]),"FUNCTION2	" + str(func2[p-len(func2)+1]))
											#data_lines = data_lines.replace("FUNC2EXPORT	" + str(func2ex[p]),"FUNC2EXPORT	" + str(func2ex[p-1]))
										else:
											data_lines = data_lines.replace("FUNCTION2	" + str(func2[p]),"FUNCTION2	" + str(func2[p+1]))
											#data_lines = data_lines.replace("FUNC2EXPORT	" + str(func2ex[p]),"FUNC2EXPORT	" + str(func2ex[p+1]))
										with open(file_name, mode = "w", encoding="UTF-8")as f:
											f.write(data_lines)

										file_name = "floyd.cpp"
										with open(file_name, encoding="UTF-8")as f:
											data_lines = f.read()
											#data_lines2 = f.read()
										if p == len(func2)-1:
											# data_lines = data_lines.replace("FUNCTION2	" + str(func2[p]),"FUNCTION2	" + str(func2[p-1]))
											data_lines = data_lines.replace("FUNC2EXPORT	" + str(func2ex[p]),"FUNC2EXPORT	" + str(func2ex[p-len(func2ex)+1]))
										else:
											# data_lines = data_lines.replace("FUNCTION2	" + str(func2[p]),"FUNCTION2	" + str(func2[p+1]))
											data_lines = data_lines.replace("FUNC2EXPORT	" + str(func2ex[p]),"FUNC2EXPORT	" + str(func2ex[p+1]))
										with open(file_name, mode = "w", encoding="UTF-8")as f:
											f.write(data_lines)

										print('\n現在'+str((h+1))+'週目の実行\n')
										subprocess.call(["g++","floyd.cpp"])
										subprocess.run('./a.out')



