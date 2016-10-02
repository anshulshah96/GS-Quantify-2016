import pandas as pd
from pandas import *
import numpy as np
import csv
import time
from datetime import datetime
import dateutil.parser as dateparser
import pickle
import os

numerical_fields = [
	'amtIssued',
	'amtOutstanding',
	'coupon',
]

categorical_fields = [
	'issuer',
	'market',
	'collateralType',
	'couponFrequency',
	'couponType',
	'industryGroup',
	'industrySector',
	'industrySubgroup',
	'maturityType',
	'securityType',
	'paymentRank',
	'ratingAgency1Rating',
	'ratingAgency2Rating',
	'ratingAgency1Watch',
	'ratingAgency2Watch',
]

bool_fields = [
	'144aFlag',
]

date_fields = [
	'issueDate',
	'maturity',
	'ratingAgency1EffectiveDate',
	'ratingAgency2EffectiveDate'
]

def get_num(s):
	x = ''.join( e for e in s if e.isdigit() or e == '.')
	if x == '':
		return np.nan
	return float(x)

def get_days(s):
	if s == 'nan':
		return np.nan
	dt = dateparser.parse( s )
	epoch = datetime(1970,1,1)
	diff = dt - epoch
	return int(diff.days)

def get_bool( s ):
	if s == 'nan':
		return np.nan
	if s.find('0') != -1:
		return 0
	else:
		return 1

def preprocess(data):
	data = clean(data)
	data = normalize(data)
	return data

def clean(data):
	print "Cleaning Data.."
	for f in numerical_fields:
		data[f] = data[f].map(str).map(get_num)
	print "Cleaned Numerical Fields"
	for f in categorical_fields:
		data[f] = data[f].map(str).map(get_num)
	print "Cleaned Categorical Fields"
	for f in date_fields:
		data[f] = data[f].map(str).map(get_days)
	print "Cleaned Date Fields"
	for f in bool_fields:
		data[f] = data[f].map(str).map(get_bool)
	print "Cleaned Bool Fields"
	print "Cleaning Completed.."
	print "Filling Missing Values with Mean.."
	data = data.fillna(data.mean(),inplace = True)
	data[categorical_fields] = data[categorical_fields].astype(int)
	data[date_fields] = data[date_fields].astype(int)
	data[bool_fields] = data[bool_fields].astype(int)
	return data
	
def normalize(data):
	df = (data - data.mean()) / (data.max() - data.min())
	return df

def prepare_data():
	print "Reading data.."
	data = pd.read_csv( 'data/Bond_Metadata.csv',index_col='isin')
	print "Pre-processing data.."
	data = preprocess(data)
	return data