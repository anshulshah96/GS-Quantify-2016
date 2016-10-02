from sklearn.cluster import KMeans
import numpy as np
from preprocess import *

def get_kmeans(arr, k):
	kmeans = KMeans(n_clusters=k, random_state=0).fit(arr)
	return kmeans

if __name__ == '__main__':
	# arr = np.array([[1, 2], [1, 4], [1, 0], [4, 2], [4, 4], [4, 0]])
	# arr = prepare_data().as_matrix()
	arr = prepare_data().values
	# print "here"
	ans = get_kmeans(arr,1000)
	print ans.labels_