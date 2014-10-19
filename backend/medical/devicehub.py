__author__ = 'cotty'
import numpy as np
from sklearn.ensemble import RandomForestClassifier

from sklearn.externals import joblib
from django.contrib.auth.models import User
from sklearn.preprocessing import Imputer
from sklearn.preprocessing import Normalizer
from rest_framework.response import Response
#not 1 = temp
#not 2 = puls

def learn(dh):
   
  return rfc 

def load(dh):
  #try:
  #rfc = joblib.load('learn/learn-' + User.username + '.pkl');
  #except IOError: 
  rfc = learn(dh) 
  return rfc


def getData(User):  
  rfc = RandomForestClassifier()
  dh = User.userprofile.devicehubproject
  imp = Imputer(missing_values=0, strategy='mean', axis=0)
  nrm = Normalizer(norm='l1', copy=1)
  pulse = dh.pulse(limit=1000)
  X_train = []
  Y_train = np.array([])
  for data in pulse[:-1]:
    X_train.append([data.get('timestamp')])
    Y_train = np.append(Y_train, data.get('value'))
  #import pdb;pdb.set_trace(); 
  rfc.fit(X_train, Y_train)  
  #pulse = dh.pulse(limit=1000)
  X_test  = pulse[-1].get('timestamp')
  Y_test  = pulse[-1].get('value')
  d = rfc.predict([X_test])
  msg = "Odihnestete"
  if abs( d - Y_test ) > 40:
      msg = "Ai putea face activitati sportive"
  data = {
      "suggestion": msg,
  }
  return data
