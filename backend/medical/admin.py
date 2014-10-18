from django.contrib import admin
from models import UserProfile, UserMedicalHistory, DeviceHubProject, Doctor

admin.site.register(UserProfile)
admin.site.register(UserMedicalHistory)
admin.site.register(DeviceHubProject)
admin.site.register(Doctor)