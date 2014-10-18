__author__ = 'cotty'
from django.contrib.auth.models import User
from rest_framework import serializers
from medical.models import UserProfile, UserMedicalHistory, SEX


class UserRecordSerializer(serializers.ModelSerializer):
    profile = serializers.PrimaryKeyRelatedField(many=True)
    medical_data = serializers.PrimaryKeyRelatedField(many=True)

    class Meta:
        model = User
        fields = ('id', 'username', 'profile', 'medical_data')


class ProfileSerializer(serializers.ModelSerializer):
    class Meta:
        model = UserProfile
        fields = ("user", )

class UserProfileSerializer(serializers.ModelSerializer):
    profile = ProfileSerializer(many=False)

    class Meta:
        model = User
        fields = ('id', 'username', 'email', 'first_name', 'last_name')