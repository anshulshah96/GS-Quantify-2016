from sklearn.cluster import KMeans
import pandas as pd
import numpy as np
import csv
from kmeans import *

#Reading Historical Trade Data
data = pd.read_csv('data/dataset.csv')
data.drop('time', axis=1, inplace=True)
data.drop('timeofday', axis=1, inplace=True)
data.drop('date', axis=1, inplace=True)
data.drop('price', axis=1, inplace=True)

#Group Data by side
gdata = data.groupby(['isin','side']).sum()
sum_of_buys = data.groupby(['side']).sum().loc['B']['volume']
sum_of_sell = data.groupby(['side']).sum().loc['S']['volume']

#Getting Clusters
arr = get_cluster()
gcl = arr.groupby('cluster')
print "Applied KMeans and Got Clusters"

#Z-Function
def z_func(a,b):
    x = (a - b)*(a - b)
    sum_sq = 0
    for j in x:
        sum_sq += j
    return sum_sq

#Map of Clusters and List of isin's
csmap = {}
isinlist = []
for i,j in gcl:
    csmap[i]=list(j.index);
    for k in j.index:
        isinlist.append(k)

print "Prediction Starts"

#Initial value
count = 0
tot_score1 = 0
tot_score2 = 0
warr = []

#For each isin i.e. bond
for k in isinlist:
    score1 = 0.0
    score2 = 0.0
    cluster = arr.loc[k]['cluster']
    bonds = csmap[cluster]
    #For each bond in the cluster of selected bond
    for bond in bonds:
        try:
            #Score = (1/(1+r^2)) where r is distance between the two points
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
    warr.append((k,score1,score2))
    
#Writing output to the file
print "Started writing CSV"
csvfile = open('output/bond_output.csv','w')
fieldnames = ['isin','buyvolume','sellvolume']
writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
writer.writeheader()
for tup in warr:
    writer.writerow({'isin':tup[0],'buyvolume':((tup[1]*sum_of_buys/tot_score1)/30),'sellvolume':((tup[2]*sum_of_sell/tot_score2)/30)})  
csvfile.close()