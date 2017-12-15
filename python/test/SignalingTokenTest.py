import sys
import unittest
import os

sys.path.append(os.path.join(os.path.dirname(__file__), '../src'))
import SignalingToken

account = "2882341273"
appID = "970CA35de60c44645bbae8a215061b33"
appCertificate = "5CFd2fd1755d40ecb72977518be15d3b"
validTimeInSeconds = 3600*24

class DynamicKeyTest(unittest.TestCase):

    def test_generate(self):
        result = SignalingToken.generateSignalingToken(
            account, appID, appCertificate, validTimeInSeconds)
        print result

if __name__ == "__main__":
    unittest.main()

