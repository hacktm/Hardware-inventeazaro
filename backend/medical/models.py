from django.db import models
from django.contrib.auth.models import User
from django.db.models.signals import post_save

# Create your models here.


SEX = (
    (1, "Male"),
    (2, "Female"),
)


class Doctor(models.Model):
    name = models.CharField(max_length=60)


class DeviceHubProject(models.Model):
    id = models.IntegerField(max_length=10, primary_key=True)
    api_key = models.CharField(max_length=64)


class UserProfile(models.Model):
    user = models.OneToOneField(User)
    age = models.IntegerField(max_length=3)
    sex = models.IntegerField(choices=SEX)
    cnp = models.IntegerField(max_length=13)  # CNP
    height = models.IntegerField(max_length=3)  # cm
    weight = models.IntegerField(max_length=3)  # kg
    family_doctor = models.ForeignKey(Doctor)
    blood_group = models.CharField(max_length=3)
    phone = models.CharField(max_length=20)

    emergency_contact = models.CharField(max_length=64)
    emergency_relation = models.CharField(max_length=64)
    emergency_phone = models.CharField(max_length=20)

    devicehub_project = models.OneToOneField(DeviceHubProject)

    gravatar_img = models.CharField(max_length=128, blank=True)

    def __unicode__(self):
        return self.user.username


class UserMedicalHistory(models.Model):
    user = models.OneToOneField(User)
    date = models.DateTimeField(auto_now_add=True)
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


def create_user_profile(sender, instance, created, **kwargs):
    """Create the UserProfile when a new User is saved"""
    if created:
        profile = UserProfile()
        profile.user = instance
        profile.save()

post_save.connect(create_user_profile, sender=User)

def create_auth_token(sender, instance=None, created=False, **kwargs):
    if created:
        Token.objects.create(user=instance)

post_save.connect(create_auth_token, sender=User)