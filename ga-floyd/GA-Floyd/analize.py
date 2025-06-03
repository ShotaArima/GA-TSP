import csv
from collections import defaultdict

# CSVファイルのパス
csv_file = 'result_list1.csv'
result_file = 'result-ave.csv'

# 条件に一致するデータの処理時間と平方根和の差を格納する辞書
data_dict = defaultdict(list)

# CSVファイルの読み込みとデータの集計
with open(csv_file, 'r') as file:
    reader = csv.reader(file, delimiter='\t')  # 区切り文字をタブ文字に変更
    next(reader)  # ヘッダー行をスキップ

    for row in reader:
        key = tuple(row[2:10])  # 条件の列をキーとして使用（選択方式と交叉方式を含む）
        processing_time = float(row[0])
        difference = float(row[1])
        data_dict[key].append((processing_time, difference))

# 条件ごとの処理時間と平方根和の差の平均を計算し、結果を新しいCSVファイルに書き込み
with open(result_file, 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(['同種の数', '処理時間の平均', '平方根和の差の平均', '世代交代数', '個体集団内の個体の数', 'エリート保存戦略で残す個体の数', 'トーナメント選択で無作為抽出する個体の数', '突然変異確率', 'フロイド問題におけるN', '選択方式', '交叉方式'])

    for key, data in data_dict.items():
        avg_processing_time = sum(d[0] for d in data) / len(data)
        avg_difference = sum(d[1] for d in data) / len(data)
        writer.writerow([len(data)] + [f'{avg_processing_time:.8f}', f'{avg_difference:.8f}'] + list(key))
