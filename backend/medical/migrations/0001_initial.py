# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations
from django.conf import settings


class Migration(migrations.Migration):

    dependencies = [
        migrations.swappable_dependency(settings.AUTH_USER_MODEL),
    ]

    operations = [
        migrations.CreateModel(
            name='DeviceHubProject',
            fields=[
                ('id', models.IntegerField(max_length=10, serialize=False, primary_key=True)),
                ('api_key', models.CharField(max_length=64)),
            ],
            options={
            },
            bases=(models.Model,),
        ),
        migrations.CreateModel(
            name='Doctor',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('name', models.CharField(max_length=60)),
            ],
            options={
            },
            bases=(models.Model,),
        ),
        migrations.CreateModel(
            name='UserMedicalHistory',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('date', models.DateTimeField(auto_now_add=True)),
                ('current_problems', models.TextField(blank=True)),
                ('regular_medication', models.TextField(blank=True)),
                ('allergies', models.TextField(blank=True)),
                ('injuries', models.TextField(blank=True)),
                ('epilepsy', models.BooleanField(default=False)),
                ('diabetes', models.BooleanField(default=False)),
                ('heart_problems', models.BooleanField(default=False)),
                ('heart_murmur', models.BooleanField(default=False)),
                ('asthma_bronchitis', models.BooleanField(default=False)),
                ('hernia', models.BooleanField(default=False)),
                ('concussion', models.BooleanField(default=False)),
                ('fracture_last_3y', models.BooleanField(default=False)),
                ('fracture_description', models.CharField(max_length=64, blank=True)),
                ('dislocation', models.BooleanField(default=False)),
                ('dislocation_description', models.CharField(max_length=64, blank=True)),
                ('recurring_pain', models.BooleanField(default=False)),
                ('recurring_pain_description', models.CharField(max_length=64, blank=True)),
                ('back_neck_pain', models.BooleanField(default=False)),
                ('glasses', models.BooleanField(default=False)),
                ('contact_lenses', models.BooleanField(default=False)),
                ('protective_equipment', models.BooleanField(default=False)),
                ('mouth_guard', models.BooleanField(default=False)),
                ('head_neck_spinal_injury', models.BooleanField(default=False)),
                ('head_neck_spinal_injury_description', models.TextField(blank=True)),
                ('user', models.OneToOneField(to=settings.AUTH_USER_MODEL)),
            ],
            options={
            },
            bases=(models.Model,),
        ),
        migrations.CreateModel(
            name='UserProfile',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('age', models.IntegerField(max_length=3)),
                ('sex', models.IntegerField(choices=[(1, b'Male'), (2, b'Female')])),
                ('cnp', models.IntegerField(max_length=13)),
                ('height', models.IntegerField(max_length=3)),
                ('weight', models.IntegerField(max_length=3)),
                ('blood_group', models.CharField(max_length=3)),
                ('phone', models.CharField(max_length=20)),
                ('emergency_contact', models.CharField(max_length=64)),
                ('emergency_relation', models.CharField(max_length=64)),
                ('emergency_phone', models.CharField(max_length=20)),
                ('gravatar_img', models.CharField(max_length=128, blank=True)),
                ('devicehub_project', models.OneToOneField(to='medical.DeviceHubProject')),
                ('family_doctor', models.ForeignKey(to='medical.Doctor')),
                ('user', models.OneToOneField(to=settings.AUTH_USER_MODEL)),
            ],
            options={
            },
            bases=(models.Model,),
        ),
    ]
