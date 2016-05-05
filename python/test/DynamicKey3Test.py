import sys
import unittest
import os

sys.path.append(os.path.join(os.path.dirname(__file__), '../src'))
from DynamicKey3 import generateDynamicKey3

statickey = "970ca35de60c44645bbae84215061b33b"
signkey = "5cfd2fd1755d40ecb72977518be15d3b"
channelname = "7d72365eb983485397e3e3f9d460bdda"
unixts = 1446455472
uid = 2882341273
randomint = 58964981
expiredts = 1446455471

class DynamicKey3Test(unittest.TestCase):
    def test_generate(self):
        expected = "00381e1588b09e6e76a0d2e8fd9e5257dbddd9c60a0970ca35de60c44645bbae84215061b33b14464554720383bbf528823412731446455471";
        actual = generateDynamicKey3(statickey, signkey, channelname, unixts, randomint, uid, expiredts);
        self.assertEqual(expected, actual)

if __name__ == "__main__":
    unittest.main()
