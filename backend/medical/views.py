from django.http import HttpResponse
from serializers import UserMedicalHistorySerializer, UserProfileSerializer
from rest_framework.response import Response
from rest_framework.views import APIView
from rest_framework.authentication import TokenAuthentication, SessionAuthentication
from rest_framework.permissions import IsAuthenticated
from rest_framework.decorators import authentication_classes, permission_classes


def index(request):
    return HttpResponse("NOT ALLOWED", status=403)


@authentication_classes((TokenAuthentication, SessionAuthentication))
@permission_classes((IsAuthenticated,))
class UserProfile(APIView):
    def get(self, request, format=None):
        user = request.user
        serializer = UserProfileSerializer(user, many=False)
        return Response(serializer.data)

    def post(self, request, format=None):
        return Response("not implemented yet", status=404)


@authentication_classes((TokenAuthentication, SessionAuthentication))
@permission_classes((IsAuthenticated,))
class UserMedicalHistory(APIView):
    def get(self, request, format=None):
        user = request.user
        serializer = UserMedicalHistorySerializer(user, many=False)
        return Response(serializer.data)

    def post(self, request, format=None):
        return Response("not implemented yet", status=404)