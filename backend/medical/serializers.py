__author__ = 'cotty'
from django.contrib.auth.models import User
from rest_framework import serializers
from medical.models import UserProfile, UserMedicalHistory, SEX


class MedicalHistorySerializer(serializers.ModelSerializer):
    class Meta:
        model = UserMedicalHistory
        # fields = ('id', 'username', 'profile', 'medical_data')


class UserMedicalHistorySerializer(serializers.ModelSerializer):
    usermedicalhistory = MedicalHistorySerializer(many=False)

    class Meta:
        model = User
        fields = ('usermedicalhistory', )


class ProfileSerializer(serializers.ModelSerializer):
    class Meta:
        model = UserProfile
        # fields = ("user", )


class UserProfileSerializer(serializers.ModelSerializer):
    userprofile = ProfileSerializer(many=False)

    class Meta:
        model = User
        fields = ('id', 'username', 'email', 'first_name', 'last_name', 'userprofile')