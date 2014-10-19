from django.http import HttpResponse
from serializers import UserMedicalHistorySerializer, UserProfileSerializer
from rest_framework.response import Response
from rest_framework.views import APIView
from rest_framework.authentication import TokenAuthentication, SessionAuthentication
from rest_framework.permissions import IsAuthenticated
from rest_framework.decorators import authentication_classes, permission_classes
from models import DeviceHubProject
from devicehub import getData


class NoHardwareEndpoint(Exception):
    message = "No hardware endpoint registered for user"


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


@authentication_classes((TokenAuthentication, SessionAuthentication))
@permission_classes((IsAuthenticated,))
class UserDataHistory(APIView):
    def get(self, request, item=None, format=None):
        user = request.user
        max_num_points = 1000
        try:
            dh = user.userprofile.devicehubproject
            if not isinstance(dh, DeviceHubProject):
                raise NoHardwareEndpoint
        except Exception as e:
            data = {
                'success': False,
                'error_msg': e.message,
            }
            return Response(data)
        else:
            data = {
                'success': True,
                'pulse': dh.pulse(limit=max_num_points),
                'temperature': dh.temperature(limit=max_num_points),
                'blood_sugar': dh.blood_sugar(limit=max_num_points),
                'ambient_temperature': dh.ambient_temperature(limit=max_num_points),
                'ambient_humidity': dh.ambient_humidity(limit=max_num_points),
                'ambient_air_quality': dh.ambient_air_quality(limit=max_num_points),
                'panic': dh.panic(limit=max_num_points),
            }
            if item is not None:
                return Response(data[item])
            else:
                return Response(data)


@authentication_classes((TokenAuthentication, SessionAuthentication))
@permission_classes((IsAuthenticated,))
class UserLatestData(APIView):
    def get(self, request, format=None):
        user = request.user
        try:
            dh = user.userprofile.devicehubproject
            if not isinstance(dh, DeviceHubProject):
                raise NoHardwareEndpoint
        except Exception as e:
            data = {
                'success': False,
                'error_msg': e.message,
            }
            return Response(data)
        else:
            data = {
                'success': True,
                'pulse': dh.pulse(),
                'temperature': dh.temperature(),
                'blood_sugar': dh.blood_sugar(),
                'ambient_temperature': dh.ambient_temperature(),
                'ambient_humidity': dh.ambient_humidity(),
                'ambient_air_quality': dh.ambient_air_quality(),
                'panic': dh.panic(),
            }
            return Response(data)


@authentication_classes((TokenAuthentication, SessionAuthentication))
@permission_classes((IsAuthenticated,))
class UserDataLearn(APIView):
    def get(self, request, format=None):
        user = request.user
        return Response(getData(user))