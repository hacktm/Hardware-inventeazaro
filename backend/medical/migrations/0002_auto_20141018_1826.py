# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('medical', '0001_initial'),
    ]

    operations = [
        migrations.AlterField(
            model_name='userprofile',
            name='age',
            field=models.IntegerField(max_length=3, blank=True),
        ),
        migrations.AlterField(
            model_name='userprofile',
            name='blood_group',
            field=models.CharField(max_length=3, blank=True),
        ),
        migrations.AlterField(
            model_name='userprofile',
            name='cnp',
            field=models.IntegerField(max_length=13, blank=True),
        ),
        migrations.AlterField(
            model_name='userprofile',
            name='devicehub_project',
            field=models.OneToOneField(blank=True, to='medical.DeviceHubProject'),
        ),
        migrations.AlterField(
            model_name='userprofile',
            name='emergency_contact',
            field=models.CharField(max_length=64, blank=True),
        ),
        migrations.AlterField(
            model_name='userprofile',
            name='emergency_phone',
            field=models.CharField(max_length=20, blank=True),
        ),
        migrations.AlterField(
            model_name='userprofile',
            name='emergency_relation',
            field=models.CharField(max_length=64, blank=True),
        ),
        migrations.AlterField(
            model_name='userprofile',
            name='family_doctor',
            field=models.ForeignKey(to='medical.Doctor', blank=True),
        ),
        migrations.AlterField(
            model_name='userprofile',
            name='height',
            field=models.IntegerField(max_length=3, blank=True),
        ),
        migrations.AlterField(
            model_name='userprofile',
            name='phone',
            field=models.CharField(max_length=20, blank=True),
        ),
        migrations.AlterField(
            model_name='userprofile',
            name='sex',
            field=models.IntegerField(blank=True, choices=[(1, b'Male'), (2, b'Female')]),
        ),
        migrations.AlterField(
            model_name='userprofile',
            name='weight',
            field=models.IntegerField(max_length=3, blank=True),
        ),
    ]
