from sklearn.cluster import KMeans
import numpy as np
from preprocess import *

def get_kmeans(arr, k):
	kmeans = KMeans(n_clusters=k, random_state=0).fit(arr)
	return kmeans

if __name__ == '__main__':
	arr = prepare_data().values
	# print "here"
	ans = get_kmeans(arr,1000)
	print ans.labels_