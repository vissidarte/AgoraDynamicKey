#! /usr/bin/python
#! -*- coding: utf-8 -*-

import sys
import unittest
import os
import time
from random import randint

sys.path.append(os.path.join(os.path.dirname(__file__), '../src'))
from SimpleTokenBuilder import *

appID = "970CA35de60c44645bbae8a215061b33"
appCertificate = "5CFd2fd1755d40ecb72977518be15d3b"
channelName = "7d72365eb983485397e3e3f9d460bdda"
uid = "2882341273"
expiredTs = 1446455471
salt = 1
ts = 1111111

def main():
  
  builder = SimpleTokenBuilder(appID, appCertificate, channelName, uid)
  builder.token.salt = salt
  builder.token.ts = ts
  builder.token.messages[kJoinChannel] = expiredTs

  '''
  print builder.token.messages
  builder.initPriviliges(ROLE_ADMIN)
  print builder.token.messages
  builder.setPrivilege(kJoinChannel, expiredTs)
  print builder.token.messages
  builder.removePrivilege(kJoinChannel)
  print builder.token.messages
  '''
  
  result = builder.buildToken()
  print "result:", result

if __name__ == "__main__":
  main()
