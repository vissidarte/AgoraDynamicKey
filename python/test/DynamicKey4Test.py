import sys
import unittest
import os

sys.path.append(os.path.join(os.path.dirname(__file__), '../src'))
from DynamicKey4 import generateRecordingKey
from DynamicKey4 import generateMediaChannelKey

statickey = "970ca35de60c44645bbae84215061b33b"
signkey = "5cfd2fd1755d40ecb72977518be15d3b"
channelname = "7d72365eb983485397e3e3f9d460bdda"
unixts = 1446455472
uid = 2882341273
randomint = 58964981
expiredts = 1446455471

class DynamicKeyTest(unittest.TestCase):
    def test_recording(self):
        expected = "0040f6609180e7967c4047f88fcc8c187e77a4d22a4970ca35de60c44645bbae84215061b33b14464554720383bbf51446455471";
        actual = generateRecordingKey(statickey, signkey, channelname, unixts, randomint, uid, expiredts);
        print actual
        self.assertEqual(expected, actual)

    def test_mediachannel(self):
        expected = "004b3e0a78e01b5a07a4627b03ff1bdcc4330839c9a970ca35de60c44645bbae84215061b33b14464554720383bbf51446455471";
        actual = generateMediaChannelKey(statickey, signkey, channelname, unixts, randomint, uid, expiredts);
        print actual
        self.assertEqual(expected, actual)

if __name__ == "__main__":
    unittest.main()
