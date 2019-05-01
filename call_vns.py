import numpy as np
from sklearn.metrics import pairwise_distances
import pandas as pd
import os

import sys

i = int(sys.argv[1])

base = 'result/' + str(i)
if not os.path.exists(base):
    os.mkdir(base)


# print("Instancia ", i)
instance = "/Users/rodrigorandel/Workspace/duals_paper2/2018-10-30_20_43/" + str(i) + '/'
# instance = "/home/costalean/rodrigo/2018-10-30_20_43/" + str(i) + '/'

""" Reading data """
data = np.load(instance + "data_x.npy")
f = open(base + "/inst.txt", "w")
f.write(str(len(data)) + " 2")
f.close()
labels = np.load(instance + "data_y.npy")
p = len(np.unique(labels))
d = pairwise_distances(data, metric='sqeuclidean')

np.savetxt(base + '/dist.txt', d, fmt='%.7f', delimiter='\t', newline='\n')

CL = pd.read_csv(instance + "cl_duals.csv", sep=',', header=None, names=["elem1", "elem2", "duals"])
CL["c"] = -1
ML = pd.read_csv(instance + "ml_duals.csv", sep=',', header=None, names=["elem1", "elem2", "duals"])
ML["c"] = 1
df = pd.concat([ML, CL])


"""
----------- PESSIMISTA ---------------
"""
print("\t----------PESSIMISTA---------")
cline = './main -i ' + base + '/inst.txt -c empty.txt -d ' + base + '/dist.txt -p ' + str(p) + ' -k 10 -t 100 -w1 -x10 > ' + base + '/out.txt'
# print(cline)
os.system(cline)

# time.sleep(2)
base_cost = float(open(base + "/out.txt").read())
# print("teste", base_cost)
# exit(1)
result = pd.DataFrame(columns=df.columns)
j = 0
for index, row in df.iterrows():
    print("\t\t" + str(int(row['elem1'])) + " " + str(int(row['elem2'])) + " " + str(int(row['c'])))
    f = open(base + "/constraints.txt", "w")
    f.write(str(int(row['elem1'])) + " " + str(int(row['elem2'])) + " " + str(int(row['c'])))
    f.close()
    cline = './main -i ' + base + '/inst.txt -c ' + base + '/constraints.txt -d ' + base + '/dist.txt -p ' + str(p) + ' -k 10 -t 100 -r1 -x10 > ' + base + '/out.txt'
    # print(cline)
    os.system(cline)
    cost = float(open(base + "/out.txt").read())

    result.loc[j] = [int(row['elem1']), int(row['elem2']), base_cost-cost, int(row['c'])]
    j += 1

result[result['c'] == 1].to_csv('result/' + str(i) + '/pessimist_ml_duals.csv', index=False)
result[result['c'] == -1].to_csv('result/' + str(i) + '/pessimist_cl_duals.csv', index=False)

"""
----------- OTIMIST ---------------
"""
print("\t----------OTIMIST---------")
f = open(base + "/all.txt", "w")
for index, row in df.iterrows():
    f.write(str(int(row['elem1'])) + " " + str(int(row['elem2'])) + " " + str(int(row['c'])) + '\n')
f.close()

cline = './main -i ' + base + '/inst.txt -c ' + base + '/all.txt -d ' + base + '/dist.txt -p ' + str(p) + ' -k 10 -t 100 -w1 -x10 > ' + base + '/out.txt'
# print(cline)
os.system(cline)
base_cost = float(open(base + "/out.txt").read())
result = pd.DataFrame(columns=df.columns)

j = 0
for index, row in df.iterrows():
    print("\t\t"+str(int(row['elem1'])) + " " + str(int(row['elem2'])) + " " + str(int(row['c'])))
    os.system('cp ' + base + '/all.txt ' + base + '/constraints.txt')
    with open(base + "/constraints.txt", "r") as f:
        lines = f.readlines()
    with open(base + "/constraints.txt", "w") as f:
        for line in lines:
            if line.strip("\n") != str(int(row['elem1'])) + " " + str(int(row['elem2'])) + " " + str(int(row['c'])):
                f.write(line)
    cline = './main -i ' + base + '/inst.txt -c ' + base + '/constraints.txt -d ' + base + '/dist.txt -p ' + str(p) + ' -k 10 -t 100 -r1 -x10 > ' + base + '/out.txt'
    # print(cline)
    os.system(cline)
    cost = float(open(base + "/out.txt").read())

    result.loc[j] = [int(row['elem1']), int(row['elem2']), base_cost - cost, int(row['c'])]
    j += 1

result[result['c'] == 1].to_csv('result/' + str(i) + '/otimist_ml_duals.csv', index=False)
result[result['c'] == -1].to_csv('result/' + str(i) + '/otimist_cl_duals.csv', index=False)
