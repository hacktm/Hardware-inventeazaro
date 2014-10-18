__author__ = 'cotty'
from django.conf.urls import patterns, include, url
from views import index, UserProfile, UserMedicalHistory

urlpatterns = patterns('',
    # Examples:
    # url(r'^$', 'backend.views.home', name='home'),
    # url(r'^blog/', include('blog.urls')),
    url(r'^$', index, name='home'),
    url(r'^auth$', 'rest_framework.authtoken.views.obtain_auth_token'),
    #url(r'^users/(?P<pk>[0-9]+)/$', UserDetail.as_view()),
    url(r'^profile$', UserProfile.as_view()),
    url(r'^medical-history', UserMedicalHistory.as_view()),
)