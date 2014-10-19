__author__ = 'cotty'
import numpy as np
from sklearn.ensemble import RandomForestClassifier

from sklearn.externals import joblib
from django.contrib.auth.models import User

#not 1 = temp
#not 2 = puls


def learn(rfc,User):
  dh = User.devicehubproject
  pulse = dh.pulse(limit=1000)
  import pdb;pdb.set_trace()


def load(rfc,User):
    try:
      joblib.load('learn/learn-' + User.username + '.pkl');
    except IOError:
      learn(rfc,User)
    return  rfc


def getData(User):
    rfc = RandomForestClassifier()
    rfc = load(rfc,User)
    joblib.dump(rfc, 'learn/learn.pkl')  
    data = {
        "suggestion": None,
    }
    return data
