from sklearn.cluster import KMeans
import numpy as np
from preprocess import *

#Applying KMeans
def get_kmeans(arr, k):
	kmeans = KMeans(n_clusters=k, random_state=0).fit(arr)
	return kmeans

#Creating Clusters
def get_cluster():
	arr = prepare_data()
	print "Starting K-Means"
	ans = get_kmeans(arr.values,1000)
	arr['cluster'] = ans.labels_
	return arr