# AgoraDynamicKey

For join media channel, use generateMediaChannelKey

For recording service, use generateRecordingKey

Here are sample for C++, java, python and nodejs

## C++
```c
/**
 * build with command:
 * g++ main.cpp  -lcrypto -std=c++0x
 */

#include "../src/DynamicKey4.h"
#include <iostream>
#include <cstdint>

int main(int argc, char const *argv[]) {
  ::srand(::time(NULL));

  auto staticKey  = "970ca35de60c44645bbae8a215061b33";
  auto  signKey   = "5cfd2fd1755d40ecb72977518be15d3b";
  auto channelName= "my channel name for recording";
  auto  unixTs = ::time(NULL);
  int randomInt = (::rand()%256 << 24) + (::rand()%256 << 16) + (::rand()%256 << 8) + (::rand()%256);
  uint32_t uid = 2882341273u;
  auto  expiredTs = 0;

  std::cout << agora::tools::DynamicKey4::generateRecordingKey(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs) << std::endl;
  std::cout << agora::tools::DynamicKey4::generateMediaChannelKey(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs) << std::endl;
  return 0;
}
```

## Java
```java
package io.agora.media.sample;

import io.agora.media.DynamicKey4;
import java.util.Date;
import java.util.Random;

public class DynamicKey4Sample {
    static String vendor   = "970ca35de60c44645bbae8a215061b33";
    static String key      = "5cfd2fd1755d40ecb72977518be15d3b";
    static String channel  = "7d72365eb983485397e3e3f9d460bdda";
    static int ts = (int)(new Date().getTime()/1000);
    static int r = new Random().nextInt();
    static long uid = 2882341273L;
    static int expiredTs = 0;

    public static void main(String[] args) throws Exception {
        System.out.println(DynamicKey4.generateMediaChannelKey(vendor, key, channel, ts, r, uid, expiredTs));
        System.out.println(DynamicKey4.generateRecordingKey(vendor, key, channel, ts, r, uid, expiredTs));
    }
}
```

## Python
```python
import sys
import os
import time
from random import randint

sys.path.append(os.path.join(os.path.dirname(__file__), '../src'))
from DynamicKey4 import generateRecordingKey
from DynamicKey4 import generateMediaChannelKey

statickey   = "970ca35de60c44645bbae8a215061b33"
signkey     = "5cfd2fd1755d40ecb72977518be15d3b"
channelname = "7d72365eb983485397e3e3f9d460bdda"
unixts = int(time.time());
uid = 2882341273
randomint = -2147483647
expiredts = 0

print "%.8x" % (randomint & 0xFFFFFFFF)

if __name__ == "__main__":
    print generateRecordingKey(statickey, signkey, channelname, unixts, randomint, uid, expiredts)
    print generateMediaChannelKey(statickey, signkey, channelname, unixts, randomint, uid, expiredts)
```

## Node.js

```javascript
var DynamicKey4 = require('../src/DynamicKey4');
var vendor  = "970ca35de60c44645bbae8a215061b33";
var key     = "5cfd2fd1755d40ecb72977518be15d3b";
var channel = "my channel name";
var ts = Math.floor(new Date() / 1000);
var r = Math.floor(Math.random() * 0xFFFFFFFF);
var uid = 2882341273;
var expiredTs = 0;


var recordingKey = DynamicKey4.generateRecordingKey(vendor, key, channel, ts, r, uid, expiredTs);
console.log(recordingKey);

var mediaChannelKey = DynamicKey4.generateMediaChannelKey(vendor, key, channel, ts, r, uid, expiredTs);
console.log(mediaChannelKey);

```
