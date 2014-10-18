# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations
import datetime


class Migration(migrations.Migration):

    dependencies = [
        ('medical', '0004_auto_20141018_1828'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='usermedicalhistory',
            name='date',
        ),
        migrations.AddField(
            model_name='usermedicalhistory',
            name='created',
            field=models.DateTimeField(default=datetime.date(2014, 10, 18), auto_now_add=True, auto_created=True),
            preserve_default=False,
        ),
        migrations.AddField(
            model_name='usermedicalhistory',
            name='updated',
            field=models.DateTimeField(default=datetime.date(2014, 10, 18), auto_now_add=True, auto_created=True),
            preserve_default=False,
        ),
    ]
