__author__ = 'cotty'
from django.contrib.auth.models import User
from rest_framework import serializers
from medical.models import UserProfile, UserMedicalHistory, SEX, Doctor, DeviceHubProject


class MedicalHistorySerializer(serializers.ModelSerializer):
    class Meta:
        model = UserMedicalHistory
        # fields = ('id', 'username', 'profile', 'medical_data')


class UserMedicalHistorySerializer(serializers.ModelSerializer):
    usermedicalhistory = MedicalHistorySerializer(many=False)

    class Meta:
        model = User
        fields = ('usermedicalhistory', )


class DoctorSerializer(serializers.ModelSerializer):
    class Meta:
        model = Doctor


class ProfileSerializer(serializers.ModelSerializer):
    sex_meta = serializers.ChoiceField(choices=SEX)
    family_doctor = DoctorSerializer(many=False)

    class Meta:
        model = UserProfile
        fields = (
            'id',
            'age',
            'sex',
            'sex_meta',
            'cnp',
            'height',
            'weight',
            'family_doctor',
            'blood_group',
            'phone',
            'emergency_contact',
            'emergency_relation',
            'emergency_phone',
            'gravatar_img',
        )
        exclude = ('devicehub_project', 'user')


class UserProfileSerializer(serializers.ModelSerializer):
    userprofile = ProfileSerializer(many=False)

    class Meta:
        model = User
        fields = ('id', 'username', 'email', 'first_name', 'last_name', 'userprofile')