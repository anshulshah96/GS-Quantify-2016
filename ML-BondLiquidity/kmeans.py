from sklearn.cluster import KMeans
import numpy as np
from preprocess import *

def get_kmeans(arr, k):
	kmeans = KMeans(n_clusters=k, random_state=0).fit(arr)
	return kmeans

def get_cluster():
	# arr = pandas.read_pickle("objects/prepared_data.p")
	# arr = prepare_data()
	# arr.to_pickle("objects/prepared_data.p")
	# print "Starting K-Means"
	# ans = get_kmeans(arr.values,1000)
	# arr['cluster'] = ans.labels_
	# arr.to_pickle("objects/clustered_data.p")
	arr = pandas.read_pickle("objects/clustered_data.p")
	return arr

if __name__ == '__main__':
	arr = get_cluster()
	# print arr.info()