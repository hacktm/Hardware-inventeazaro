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

    def __unicode__(self):
        return self.user.username


def create_user_profile(sender, instance, created, **kwargs):
    """Create the UserProfile when a new User is saved"""
    if created:
        profile = UserProfile()
        profile.user = instance
        profile.save()

post_save.connect(create_user_profile, sender=User)