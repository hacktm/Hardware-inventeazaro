__author__ = 'cotty'
from models import DeviceHubProject
import numpy as np
from sklearn.ensemble import RandomForestClassifier


def ml_test():
    learn_pulse = np.array([73, 80, 84, 87, 81, 74, 80, 87, 92, 90, 75, 90, 94, 87, 84])
    rfc = RandomForestClassifier()
    rfc.fit_transform(np.array([learn_pulse, [1]]))
    rfc.predict(np.array([75], [1]))
    




