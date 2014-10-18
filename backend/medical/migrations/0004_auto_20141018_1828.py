# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('medical', '0003_auto_20141018_1827'),
    ]

    operations = [
        migrations.AlterField(
            model_name='userprofile',
            name='blood_group',
            field=models.CharField(max_length=3, null=True, blank=True),
        ),
        migrations.AlterField(
            model_name='userprofile',
            name='cnp',
            field=models.IntegerField(max_length=13, null=True, blank=True),
        ),
        migrations.AlterField(
            model_name='userprofile',
            name='devicehub_project',
            field=models.OneToOneField(null=True, blank=True, to='medical.DeviceHubProject'),
        ),
        migrations.AlterField(
            model_name='userprofile',
            name='emergency_contact',
            field=models.CharField(max_length=64, null=True, blank=True),
        ),
        migrations.AlterField(
            model_name='userprofile',
            name='emergency_phone',
            field=models.CharField(max_length=20, null=True, blank=True),
        ),
        migrations.AlterField(
            model_name='userprofile',
            name='emergency_relation',
            field=models.CharField(max_length=64, null=True, blank=True),
        ),
        migrations.AlterField(
            model_name='userprofile',
            name='family_doctor',
            field=models.ForeignKey(blank=True, to='medical.Doctor', null=True),
        ),
        migrations.AlterField(
            model_name='userprofile',
            name='gravatar_img',
            field=models.CharField(max_length=128, null=True, blank=True),
        ),
        migrations.AlterField(
            model_name='userprofile',
            name='height',
            field=models.IntegerField(max_length=3, null=True, blank=True),
        ),
        migrations.AlterField(
            model_name='userprofile',
            name='phone',
            field=models.CharField(max_length=20, null=True, blank=True),
        ),
        migrations.AlterField(
            model_name='userprofile',
            name='sex',
            field=models.IntegerField(blank=True, null=True, choices=[(1, b'Male'), (2, b'Female')]),
        ),
        migrations.AlterField(
            model_name='userprofile',
            name='weight',
            field=models.IntegerField(max_length=3, null=True, blank=True),
        ),
    ]
