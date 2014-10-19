from django.core.management.base import BaseCommand, CommandError
from medical.models import UserProfile
from django_gravatar.helpers import get_gravatar_url, has_gravatar, get_gravatar_profile_url, calculate_gravatar_hash


class Command(BaseCommand):
    help = 'Update gravatar information for registered users in the system'

    def handle(self, *args, **options):
        for user_det in UserProfile.objects.all():
            if has_gravatar(user_det.user.email):
                user_det.gravatar_img = get_gravatar_url(user_det.user.email, size=200)
            else:
                user_det.gravatar_img = "http://dummyimage.com/200x200/fff/000.png&text=" + str(user_det.user.username[0:2])
            user_det.save()

        print "Updated gravatars!"
