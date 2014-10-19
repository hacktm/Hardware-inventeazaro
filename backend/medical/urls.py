__author__ = 'cotty'
from django.conf.urls import patterns, url
from views import index, UserProfile, UserMedicalHistory, UserLatestData, UserDataHistory, UserDataLearn

urlpatterns = patterns('',
    # Examples:
    # url(r'^$', 'backend.views.home', name='home'),
    # url(r'^blog/', include('blog.urls')),
    url(r'^$', index, name='home'),
    url(r'^auth$', 'rest_framework.authtoken.views.obtain_auth_token'),
    #url(r'^users/(?P<pk>[0-9]+)/$', UserDetail.as_view()),
    url(r'^profile$', UserProfile.as_view()),
    url(r'^medical-history', UserMedicalHistory.as_view()),
    url(r'^data-latest', UserLatestData.as_view()),
    url(r'^data-history?(?:/(?P<item>\w+))?$', UserDataHistory.as_view()),
    url(r'^data-learn', UserDataLearn.as_view()),
)
