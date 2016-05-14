import sys
import unittest
import os
import time
from random import randint

sys.path.append(os.path.join(os.path.dirname(__file__), '../src'))
from DynamicKey4 import generateRecordingKey
from DynamicKey4 import generateMediaChannelKey

statickey   = "970ca35de60c44645bbae8a215061b33"
signkey     = "5cfd2fd1755d40ecb72977518be15d3b"
channelname = "7d72365eb983485397e3e3f9d460bdda"
unixts = 1463199341
uid = 2882341273
randomint = -252347445
expiredts = 0

class DynamicKeyTest(unittest.TestCase):
    def test_recording(self):
        expected = "004f0386ad262b31103ec737f06c303f778c2410fed970ca35de60c44645bbae8a215061b331463199341f0f57bcb0000000000";
        actual = generateRecordingKey(statickey, signkey, channelname, unixts, randomint, uid, expiredts);
        print actual
        self.assertEqual(expected, actual)

    def test_mediachannel(self):
        expected = "0043d8758ab4f38a56400c7bbfaae66d7ff4b5bea26970ca35de60c44645bbae8a215061b331463199341f0f57bcb0000000000";
        actual = generateMediaChannelKey(statickey, signkey, channelname, unixts, randomint, uid, expiredts);
        print actual
        self.assertEqual(expected, actual)

if __name__ == "__main__":
    unittest.main()
