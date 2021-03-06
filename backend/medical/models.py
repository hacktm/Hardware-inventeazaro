from django.db import models
from django.contrib.auth.models import User
from django.db.models.signals import post_save
from rest_framework.authtoken.models import Token
from django.conf import settings
import httplib2
import json
from django_gravatar.helpers import get_gravatar_url


SEX = (
    (1, "Male"),
    (2, "Female"),
)


class Doctor(models.Model):
    name = models.CharField(max_length=60)

    def __unicode__(self):
        return self.name


class UserProfile(models.Model):
    user = models.OneToOneField(User)
    age = models.IntegerField(max_length=3, blank=True, null=True)
    sex = models.IntegerField(choices=SEX, blank=True, null=True)
    cnp = models.IntegerField(max_length=13, blank=True, null=True)  # CNP
    height = models.IntegerField(max_length=3, blank=True, null=True)  # cm
    weight = models.IntegerField(max_length=3, blank=True, null=True)  # kg
    family_doctor = models.ForeignKey(Doctor, blank=True, null=True)
    blood_group = models.CharField(max_length=3, blank=True, null=True)
    phone = models.CharField(max_length=20, blank=True, null=True)

    emergency_contact = models.CharField(max_length=64, blank=True, null=True)
    emergency_relation = models.CharField(max_length=64, blank=True, null=True)
    emergency_phone = models.CharField(max_length=20, blank=True, null=True)

    # devicehub_project = models.OneToOneField(DeviceHubProject, blank=True, null=True)

    gravatar_img = models.CharField(max_length=128, blank=True, null=True)

    @property
    def sex_meta(self):
        return SEX

    def __unicode__(self):
        return self.user.username


class UserMedicalHistory(models.Model):
    user = models.OneToOneField(User)
    created = models.DateTimeField(auto_now_add=True, auto_created=True)
    updated = models.DateTimeField(auto_now_add=True, auto_created=True)
    current_problems = models.TextField(blank=True)
    regular_medication = models.TextField(blank=True)
    allergies = models.TextField(blank=True)
    injuries = models.TextField(blank=True)
    # Have/Had
    epilepsy = models.BooleanField(default=False)
    diabetes = models.BooleanField(default=False)
    heart_problems = models.BooleanField(default=False)
    heart_murmur = models.BooleanField(default=False)
    asthma_bronchitis = models.BooleanField(default=False)
    hernia = models.BooleanField(default=False)
    concussion = models.BooleanField(default=False)
    fracture_last_3y = models.BooleanField(default=False)
    fracture_description = models.CharField(max_length=64, blank=True)
    dislocation = models.BooleanField(default=False)
    dislocation_description = models.CharField(max_length=64, blank=True)
    recurring_pain = models.BooleanField(default=False)  # recurring pain with play/practice
    recurring_pain_description = models.CharField(max_length=64, blank=True)
    back_neck_pain = models.BooleanField(default=False)

    # Wearing
    glasses = models.BooleanField(default=False)
    contact_lenses = models.BooleanField(default=False)
    protective_equipment = models.BooleanField(default=False)
    mouth_guard = models.BooleanField(default=False)

    # Treated for
    head_neck_spinal_injury = models.BooleanField(default=False)
    head_neck_spinal_injury_description = models.TextField(blank=True)

    def __unicode__(self):
        return self.user.username


class DeviceHubProject(models.Model):
    userprofile = models.OneToOneField(UserProfile)
    project_id = models.IntegerField(max_length=10, default=453)
    api_key = models.CharField(max_length=64, default="afb7baff-25c4-42cd-9795-56ccdfff4c35")

    def get_latest_sensor_value(self, sensor_id, limit=1):
        get_url = settings.DEVICEHUB_API_BASE + \
                    'project/' + str(self.project_id) + \
                    '/sensor/' + str(sensor_id) + \
                    '/?limit=' + str(limit) + \
                    '&apiKey=' + self.api_key

        h = httplib2.Http()
        resp, content = h.request(get_url, "GET")
        content = json.loads(content)
        content = [{'timestamp': x['timestamp'], 'value': x['value']} for x in content]
        if len(content) == 1:
            return content[0]
        else:
            return content

    def pulse(self, limit=1):
        return self.get_latest_sensor_value(876, limit)

    def ambient_temperature(self, limit=1):
        return self.get_latest_sensor_value(877, limit)

    def ambient_humidity(self, limit=1):
        return self.get_latest_sensor_value(878, limit)

    def ambient_air_quality(self, limit=1):
        return self.get_latest_sensor_value(879, limit)

    def temperature(self, limit=1):
        return self.get_latest_sensor_value(880, limit)

    def blood_sugar(self, limit=1):
        return self.get_latest_sensor_value(881, limit)

    def panic(self, limit=1):
        return self.get_latest_sensor_value(882, limit)

    def __unicode__(self):
        return str(self.id)


def create_user_profile(sender, instance, created, **kwargs):
    """Create the UserProfile when a new User is saved"""
    if created:
        profile = UserProfile()
        profile.user = instance
        profile.gravatar_img = get_gravatar_url(profile.user.email)
        profile.save()

post_save.connect(create_user_profile, sender=User)


def create_auth_token(sender, instance=None, created=False, **kwargs):
    if created:
        Token.objects.create(user=instance)

post_save.connect(create_auth_token, sender=User)