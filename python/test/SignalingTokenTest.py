import sys
import unittest
import os
sys.path.append(os.path.join(os.path.dirname(__file__), '../src'))
import SignalingToken

appID = "970ca35de60c44645bbae8a215061b33"
appCertificate = "5cfd2fd1755d40ecb72977518be15d3b"
account = "TestAccount"
expiredTs = 1446455471


class SignalingTokenTest(unittest.TestCase):
    def test_signalingToken(self):
        result = SignalingToken.getToken(account, appID, appCertificate, expiredTs)
        expected = '1:970ca35de60c44645bbae8a215061b33:1446455471:4815d52c4fd440bac35b981c12798774'
        self.assertEqual(result, expected)


if __name__ == "__main__":
    unittest.main()
