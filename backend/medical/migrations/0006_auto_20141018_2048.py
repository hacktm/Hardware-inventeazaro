# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('medical', '0005_auto_20141018_1851'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='userprofile',
            name='devicehub_project',
        ),
        migrations.AddField(
            model_name='devicehubproject',
            name='userprofile',
            field=models.OneToOneField(default=None, to='medical.UserProfile'),
            preserve_default=False,
        ),
    ]
