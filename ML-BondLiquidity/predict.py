import pandas as pd
from pandas import *
import numpy as np
import csv
from sklearn.cluster import KMeans
import numpy as np
from kmeans import *
import random

data = pd.read_csv('data/dataset.csv')
data.drop('time', axis=1, inplace=True)
data.drop('timeofday', axis=1, inplace=True)
data.drop('date', axis=1, inplace=True)
data.drop('price', axis=1, inplace=True)
# data.info()
gdata = data.groupby(['isin','side']).sum()
sum_of_buys = data.groupby(['side']).sum().loc['B']['volume']
sum_of_sell = data.groupby(['side']).sum().loc['S']['volume']
# gdata.loc['isin10033','volume']['B']

print "Data Loaded and Cleaned"

from kmeans import *
arr = get_cluster()

print "Applied KMeans and Got Clusters"

clist = arr[['cluster']]
gcl = clist.groupby('cluster')

csmap = {}
isinlist = []
for i,j in gcl:
	csmap[i]=list(j.index);
	for k in j.index:
		isinlist.append(k)

def z_func(a,b):
    x = (a - b)*(a - b)
    sum_sq = 0
    for j in x:
        sum_sq += j
    return sum_sq

# Prediction starts
print "Prediction Starts"

count = 0
warr = []
tot_score1 = 0
tot_score2 = 0
for k in isinlist:
	score1 = 0.0
	score2 = 0.0
	cluster = arr.loc[k]['cluster']
	bonds = csmap[cluster]
	for bond in bonds:
		try:
			temp = z_func( arr.loc[bond], arr.loc[k] )
			score1 = score1 + (1 / (1 + temp ))*gdata.loc[bond,'volume']['B']
			score2 = score2 + (1 / (1 + temp ))*gdata.loc[bond,'volume']['S']
		except Exception as e:
			pass
	tot_score1 += score1
	tot_score2 += score2
	if(count % 100 == 0):
		print count, score1, score2
	count+=1
	warr.append((k,3*score1,3*score2))
	
print "Started writing CSV"
csvfile = open('names_prefinal.csv','w')
fieldnames = ['isin','buyvolume','sellvolume']
writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
writer.writeheader()
for tup in warr:
	writer.writerow({'isin':tup[0],'buyvolume':((tup[1]*sum_of_buys/tot_score1)/30),'sellvolume':((tup[2]*sum_of_sell/tot_score2)/30)})  
csvfile.close()