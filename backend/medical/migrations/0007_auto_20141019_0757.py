# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('medical', '0006_auto_20141018_2048'),
    ]

    operations = [
        migrations.AddField(
            model_name='devicehubproject',
            name='project_id',
            field=models.IntegerField(default=453, max_length=10),
            preserve_default=True,
        ),
        migrations.AlterField(
            model_name='devicehubproject',
            name='api_key',
            field=models.CharField(default=b'afb7baff-25c4-42cd-9795-56ccdfff4c35', max_length=64),
        ),
        migrations.AlterField(
            model_name='devicehubproject',
            name='id',
            field=models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True),
        ),
    ]
