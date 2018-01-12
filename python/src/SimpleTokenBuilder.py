import random
import time
import os
import sys
from collections import OrderedDict
sys.path.append(os.path.join(os.path.dirname(__file__), '../src'))
from AccessToken import *


ROLE_ATTENDEE = 1
ROLE_PUBLISHER = 2
ROLE_SUBSCRIBER = 3
ROLE_ADMIN = 4

AttendeePrivileges = OrderedDict ([
	(kJoinChannel, 0),
	(kPublishAudioStream, 0),
	(kPublishVideoStream, 0),
	(kPublishDataStream, 0)
])

PublisherPrivileges = OrderedDict ([
	(kJoinChannel, 0),
	(kPublishAudioStream, 0), 
	(kPublishVideoStream, 0),
	(kPublishDataStream, 0),
	(kPublishAudiocdn, 0),
	(kPublishVideoCdn, 0),
	(kInvitePublishAudioStream, 0),
	(kInvitePublishVideoStream, 0),
	(kAdministrateChannel, 0)
])

SubscriberPrivileges = OrderedDict ([
	(kJoinChannel, 0),
	(kRequestPublishAudioStream, 0),
	(kRequestPublishVideoStream, 0),
	(kRequestPublishDataStream, 0)
])

AdminPrivileges = OrderedDict ([
	(kJoinChannel, 0),
	(kPublishAudioStream, 0), 
	(kPublishVideoStream, 0),
	(kPublishDataStream, 0),
	(kAdministrateChannel, 0)
])

RolePrivileges = OrderedDict ([
	(ROLE_ATTENDEE, AttendeePrivileges),
	(ROLE_PUBLISHER, PublisherPrivileges),
	(ROLE_SUBSCRIBER, SubscriberPrivileges), 
	(ROLE_ADMIN, AdminPrivileges)
])

class SimpleTokenBuilder:

    def __init__(self, appID, appCertificate, channelName, uid):   
        self.token = AccessToken(appID, appCertificate, channelName, uid)
        random.seed(time.time())
        self.token.ts = int(time.time()) + 24 * 3600
        self.token.salt = random.randint(1, 99999999)
        self.token.messages = {}

    def initPriviliges(self, role) :
	    self.token.messages = RolePrivileges[role]

    def setPrivilege(self, privilege, timeoutFromNow) :
	    self.token.messages[privilege] = timeoutFromNow

    def removePrivilege(self, privilege) :
	    self.token.messages.pop(privilege)

    def buildToken(self):
        return self.token.build()
