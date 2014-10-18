from django.contrib.auth.models import User
from django.http import HttpResponse
from django.views.decorators.csrf import csrf_exempt
from rest_framework.renderers import JSONRenderer
from rest_framework.parsers import JSONParser
from models import UserProfile, UserMedicalHistory
from serializers import UserRecordSerializer, UserProfileSerializer
from rest_framework import generics
from rest_framework.views import APIView
from rest_framework.authentication import TokenAuthentication, SessionAuthentication
from rest_framework.permissions import IsAuthenticated
from rest_framework.decorators import authentication_classes, permission_classes


def index(request):
    return HttpResponse("NOT ALLOWED", status=403)


class JSONResponse(HttpResponse):
    """
    An HttpResponse that renders its content into JSON.
    """
    def __init__(self, data, **kwargs):
        content = JSONRenderer().render(data)
        kwargs['content_type'] = 'application/json'
        super(JSONResponse, self).__init__(content, **kwargs)


class UserDetail(generics.RetrieveAPIView):
    queryset = User.objects.all()
    serializer_class = UserRecordSerializer


@authentication_classes((TokenAuthentication, SessionAuthentication))
@permission_classes((IsAuthenticated,))
class UserProfile(APIView):
    serializer_class = UserProfileSerializer

    def get(self, request, format=None):
        user = User.objects.get(pk=1)
        serializer = UserProfileSerializer(user, many=False)
        return JSONResponse(serializer.data)

