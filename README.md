# AgoraDynamicKey

For join media channel, use generateMediaChannelKey

For recording service, use generateRecordingKey

Here are sample for C++, java, python and nodejs

## SDK and Dynamic Key Compatibility
Note: It is recommended for you to upgrade to DynamicKey4 ASAP.
### If you need to verify the User ID:

| Dynamic Key Version | UID | SDK Version  |
|---|---|---|
| DynamicKey4  | specify uid of user | 1.3.0 or later  |
| DynamicKey3  | specify uid of user  | 1.2.3 or later  |
| DynamicKey  |  NA |  NA |

### If you do not need to verify the User ID:

| Dynamic Key Version | UID | SDK Version  |
|---|---|---|
| DynamicKey4  | 0 | All |
| DynamicKey3  | 0 | All |
| DynamicKey  | All | All |



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

  auto appID  = "970ca35de60c44645bbae8a215061b33";
  auto  appCertificate   = "5cfd2fd1755d40ecb72977518be15d3b";
  auto channelName= "my channel name for recording";
  auto  unixTs = ::time(NULL);
  int randomInt = (::rand()%256 << 24) + (::rand()%256 << 16) + (::rand()%256 << 8) + (::rand()%256);
  uint32_t uid = 2882341273u;
  auto  expiredTs = 0;

  std::cout << agora::tools::DynamicKey4::generateRecordingKey(appID, appCertificate, channelName, unixTs, randomInt, uid, expiredTs) << std::endl;
  std::cout << agora::tools::DynamicKey4::generateMediaChannelKey(appID, appCertificate, channelName, unixTs, randomInt, uid, expiredTs) << std::endl;
  std::cout << agora::tools::DynamicKey4::generatePublicSharing(appID, appCertificate, channelName, unixTs, randomInt, uid, expiredTs) << std::endl;
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
    static String appID= "970ca35de60c44645bbae8a215061b33";
    static String appCertificate= "5cfd2fd1755d40ecb72977518be15d3b";
    static String channel  = "7d72365eb983485397e3e3f9d460bdda";
    static int ts = (int)(new Date().getTime()/1000);
    static int r = new Random().nextInt();
    static long uid = 2882341273L;
    static int expiredTs = 0;

    public static void main(String[] args) throws Exception {
        System.out.println(DynamicKey4.generateMediaChannelKey(appID, appCertificate, channel, ts, r, uid, expiredTs));
        System.out.println(DynamicKey4.generateRecordingKey(appID, appCertificate, channel, ts, r, uid, expiredTs));
        System.out.println(DynamicKey4.generatePublicSharing(appID, appCertificate, channel, ts, r, uid, expiredTs));
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
from DynamicKey4 import generatePublicSharing   

appID   = "970ca35de60c44645bbae8a215061b33"
appCertificate     = "5cfd2fd1755d40ecb72977518be15d3b"
channelname = "7d72365eb983485397e3e3f9d460bdda"
unixts = int(time.time());
uid = 2882341273
randomint = -2147483647
expiredts = 0

print "%.8x" % (randomint & 0xFFFFFFFF)

if __name__ == "__main__":
    print generateRecordingKey(appID, appCertificate, channelname, unixts, randomint, uid, expiredts)
    print generateMediaChannelKey(appID, appCertificate, channelname, unixts, randomint, uid, expiredts)
    print generatePublicSharing(appID, appCertificate, channelname, unixts, randomint, uid, expiredts)
```

## Node.js

```javascript
var DynamicKey4 = require('../src/DynamicKey4');
var appID  = "970ca35de60c44645bbae8a215061b33";
var appCertificate     = "5cfd2fd1755d40ecb72977518be15d3b";
var channel = "my channel name";
var ts = Math.floor(new Date() / 1000);
var r = Math.floor(Math.random() * 0xFFFFFFFF);
var uid = 2882341273;
var expiredTs = 0;


var recordingKey = DynamicKey4.generateRecordingKey(appID, appCertificate, channel, ts, r, uid, expiredTs);
console.log(recordingKey);

var mediaChannelKey = DynamicKey4.generateMediaChannelKey(appID, appCertificate, channel, ts, r, uid, expiredTs);
console.log(mediaChannelKey);

var publicSharingKey = DynamicKey4.generatePublicSharing(appID, appCertificate, channel, ts, r, uid, expiredTs);
console.log(publicSharingKey );
```

## go

```go
import (
	"../src/DynamicKey4"
    "fmt"
)
appID:="970ca35de60c44645bbae8a215061b33"
appCertificate:="5cfd2fd1755d40ecb72977518be15d3b"
channelName := "7d72365eb983485397e3e3f9d460bdda"
unixTs:=uint32(1446455472)
uid:=uint32(2882341273)
randomInt:=uint32(58964981)
expiredTs:=uint32(1446455471)
var publicSharingKey = DynamicKey4.GeneratePublicSharingKey(appID, appCertificate, channelName, unixTs, randomInt, uid, expiredTs)
var mediaChannelKey = DynamicKey4.GenerateMediaChannelKey(appID, appCertificate, channelName, unixTs, randomInt, uid, expiredTs)
var recordingKey = DynamicKey4.GenerateRecordingKey(appID, appCertificate, channelName, unixTs, randomInt, uid, expiredTs)
fmt.Println(publicSharingKey)
fmt.Println(mediaChannelKey)
fmt.Println(recordingKey)
```
