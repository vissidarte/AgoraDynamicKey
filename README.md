# AgoraDynamicKey

This page provides sample codes for generating AccessToken and Dynamic Key. 

+ For users using Agora SDK v 2.1 or later, see AccessToken. 
+ For users using Agora SDK v2.0.2 or earlier, see Dynamic Key. 

## AccessToken

AccessToken is more powerful than the original Dynamic Key. It can include several privileges in one token which can be sent to various services from Agora.

AccessToken is available for Agora SDK 2.1.0 or later.

### Go

You can use either of the following files to to generate an AccessToken.

+ AccessToken.go  https://github.com/AgoraIO/AgoraDynamicKey/blob/master/go/src/AccessToken/AccessToken.go
+ SimpleTokenBuilder.go https://github.com/AgoraIO/AgoraDynamicKey/blob/master/go/src/SimpleTokenBuilder/SimpleTokenBuilder.go

### C++

You can use either of the following files to to generate an AccessToken.

+ AccessToken.h  https://github.com/AgoraIO/AgoraDynamicKey/blob/master/cpp/src/AccessToken.h
+ SimpleTokenBuilder.h https://github.com/AgoraIO/AgoraDynamicKey/blob/master/cpp/src/SimpleTokenBuilder.h

### Java

You can use either of the following files to to generate an AccessToken.

+ AccessToken.java  https://github.com/AgoraIO/AgoraDynamicKey/blob/master/java/src/io/agora/media/AccessToken.java
+ SimpleTokenBuilder.java https://github.com/AgoraIO/AgoraDynamicKey/blob/master/java/src/io/agora/media/SimpleTokenBuilder.java

### Node.js

+ AccessToken.js  https://github.com/AgoraIO/AgoraDynamicKey/blob/master/nodejs/src/AccessToken.js
+ SimpleTokenBuilder.js https://github.com/AgoraIO/AgoraDynamicKey/blob/master/nodejs/src/SimpleTokenBuilder.js

### PHP

+ AccessToken.php https://github.com/AgoraIO/AgoraDynamicKey/blob/master/php/src/AccessToken.php
+ SimpleTokenBuilder.php https://github.com/AgoraIO/AgoraDynamicKey/blob/master/php/src/SimpleTokenBuilder.php

### Python

+ AccessToken.py https://github.com/AgoraIO/AgoraDynamicKey/blob/master/python/src/AccessToken.py
+ SimpleTokenBuilder.py https://github.com/AgoraIO/AgoraDynamicKey/blob/master/python/src/SimpleTokenBuilder.py




## Dynamic Key

To join a media channel, use generateMediaChannelKey.

For recording services, use generateRecordingKey.

Following are samples for C++, Go, Java, Nodejs, PHP and Python.

Note: Agora recommends that you \upgrade to DynamicKey5 ASAP.

+ If you need to verify user permission in channel:

| Dynamic Key Version | UID | SDK Version  |
|---|---|---|
| DynamicKey5  | specify the permission | 1.7.0 or later  |


+ If you need to verify the User ID:

| Dynamic Key Version | UID | SDK Version  |
|---|---|---|
| DynamicKey5  | specify uid of user | 1.3.0 or later  |
| DynamicKey4  | specify uid of user | 1.3.0 or later  |
| DynamicKey3  | specify uid of user  | 1.2.3 or later  |
| DynamicKey  |  NA |  NA |

+ If you do not need to verify the User ID:

| Dynamic Key Version | UID | SDK Version  |
|---|---|---|
| DynamicKey5  | 0 | All |
| DynamicKey4  | 0 | All |
| DynamicKey3  | 0 | All |
| DynamicKey  | All | All |



### C++
```c
/**
 * build with command:
 * g++ main.cpp  -lcrypto -std=c++0x
 */
#include "../src/DynamicKey5.h"
#include <iostream>
#include <cstdint>
using namespace agora::tools;

int main(int argc, char const *argv[]) {
  ::srand(::time(NULL));

  auto appID  = "970ca35de60c44645bbae8a215061b33";
  auto  appCertificate   = "5cfd2fd1755d40ecb72977518be15d3b";
  auto channelName= "my channel name for recording";
  auto  unixTs = ::time(NULL);
  int randomInt = (::rand()%256 << 24) + (::rand()%256 << 16) + (::rand()%256 << 8) + (::rand()%256);
  uint32_t uid = 2882341273u;
  auto  expiredTs = 0;

  std::cout << std::endl;
  std::cout << DynamicKey5::generateMediaChannelKey(appID, appCertificate, channelName, unixTs, randomInt, uid, expiredTs) << std::endl;
  std::cout << DynamicKey5::generateRecordingKey(appID, appCertificate, channelName, unixTs, randomInt, uid, expiredTs) << std::endl;
  std::cout << DynamicKey5::generateInChannelPermissionKey(appID, appCertificate, channelName, unixTs, randomInt, uid, expiredTs, DynamicKey5::noUpload()) << std::endl;
  std::cout << DynamicKey5::generateInChannelPermissionKey(appID, appCertificate, channelName, unixTs, randomInt, uid, expiredTs, DynamicKey5::audioVideoUpload()) << std::endl;

  return 0;
}

```

### Go
```go
package main

import (
    "../src/DynamicKey5"
    "fmt"
)

func main() {
    appID:="970ca35de60c44645bbae8a215061b33"
    appCertificate:="5cfd2fd1755d40ecb72977518be15d3b"
    channelName := "7d72365eb983485397e3e3f9d460bdda"
    unixTs:=uint32(1446455472)
    uid:=uint32(2882341273)
    randomInt:=uint32(58964981)
    expiredTs:=uint32(1446455471)
    var publicSharingKey,sharingError = DynamicKey5.GeneratePublicSharingKey(appID, appCertificate, channelName, unixTs, randomInt, uid, expiredTs)
    if sharingError == nil {
        fmt.Println(publicSharingKey)
    }

    var mediaChannelKey,channelError = DynamicKey5.GenerateMediaChannelKey(appID, appCertificate, channelName, unixTs, randomInt, uid, expiredTs)
    if channelError == nil {
        fmt.Println(mediaChannelKey)
    }

    var recordingKey,recordingError = DynamicKey5.GenerateRecordingKey(appID, appCertificate, channelName, unixTs, randomInt, uid, expiredTs)
    if recordingError == nil {
        fmt.Println(recordingKey)
    }

    var noUploadKey,noUploadError = DynamicKey5.GenerateInChannelPermissionKey(appID, appCertificate, channelName, unixTs, randomInt, uid, expiredTs, DynamicKey5.NoUpload)
    if noUploadError == nil {
        fmt.Println(noUploadKey)
    }

    var audioVideoUploadKey,audioVideoUploadError = DynamicKey5.GenerateInChannelPermissionKey(appID, appCertificate, channelName, unixTs, randomInt, uid, expiredTs, DynamicKey5.AudioVideoUpload)
    if audioVideoUploadError == nil {
        fmt.Println(audioVideoUploadKey)
    }
}
```

### Java
```java
package io.agora.media.sample;

import io.agora.media.DynamicKey5;

import java.util.Date;
import java.util.Random;

public class DynamicKey5Sample {
    static String appID = "970ca35de60c44645bbae8a215061b33";
    static String appCertificate = "5cfd2fd1755d40ecb72977518be15d3b";
    static String channel = "7d72365eb983485397e3e3f9d460bdda";
    static int ts = (int)(new Date().getTime()/1000);
    static int r = new Random().nextInt();
    static long uid = 2882341273L;
    static int expiredTs = 0;

    public static void main(String[] args) throws Exception {
        System.out.println(DynamicKey5.generateMediaChannelKey(appID, appCertificate, channel, ts, r, uid, expiredTs));
        System.out.println(DynamicKey5.generateRecordingKey(appID, appCertificate, channel, ts, r, uid, expiredTs));
        System.out.println(DynamicKey5.generateInChannelPermissionKey(appID, appCertificate, channel, ts, r, uid, expiredTs, DynamicKey5.noUpload));
        System.out.println(DynamicKey5.generateInChannelPermissionKey(appID, appCertificate, channel, ts, r, uid, expiredTs, DynamicKey5.audioVideoUpload));
    }
}
```

### Node.js
```javascript
var DynamicKey5 = require('../src/DynamicKey5');
var appID  = "970ca35de60c44645bbae8a215061b33";
var appCertificate     = "5cfd2fd1755d40ecb72977518be15d3b";
var channel = "my channel name";
var ts = Math.floor(new Date() / 1000);
var r = Math.floor(Math.random() * 0xFFFFFFFF);
var uid = 2882341273;
var expiredTs = 0;

console.log("5 recording key: " + DynamicKey5.generateRecordingKey(appID, appCertificate, channel, ts, r, uid, expiredTs));
console.log("5 channel key: " + DynamicKey5.generateMediaChannelKey(appID, appCertificate, channel, ts, r, uid, expiredTs));
console.log("5 in channel permission key(no upload): "
    + DynamicKey5.generateInChannelPermissionKey(appID, appCertificate, channel, ts, r, uid, expiredTs, DynamicKey5.noUpload));
console.log("5 in channel permission key(audio video upload): "
    + DynamicKey5.generateInChannelPermissionKey(appID, appCertificate, channel, ts, r, uid, expiredTs, DynamicKey5.audioVideoUpload));
```

### PHP
```php
<?php
include "../src/DynamicKey5.php";

$appID = '970ca35de60c44645bbae8a215061b33';
$appCertificate = '5cfd2fd1755d40ecb72977518be15d3b';
$channelName = "7d72365eb983485397e3e3f9d460bdda";
$ts = 1446455472;
$randomInt = 58964981;
$uid = 2882341273;
$expiredTs = 1446455471;

echo generateRecordingKey($appID, $appCertificate, $channelName, $ts, $randomInt, $uid, $expiredTs) . "\n";
echo generateMediaChannelKey($appID, $appCertificate, $channelName, $ts, $randomInt, $uid, $expiredTs) . "\n";

global $NO_UPLOAD;

echo generateInChannelPermissionKey($appID, $appCertificate, $channelName, $ts, $randomInt, $uid, $expiredTs, $NO_UPLOAD) . "\n";

global $AUDIO_VIDEO_UPLOAD;
echo generateInChannelPermissionKey($appID, $appCertificate, $channelName, $ts, $randomInt, $uid, $expiredTs, $AUDIO_VIDEO_UPLOAD) . "\n";
?>

```

### Python
```python
import sys
import os
import time
from random import randint

sys.path.append(os.path.join(os.path.dirname(__file__), '../src'))
from DynamicKey5 import *

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
    print generatePublicSharingKey(appID, appCertificate, channelname, unixts, randomint, uid, expiredts)
    print generateInChannelPermissionKey(appID, appCertificate, channelname, unixts, randomint, uid, expiredts, NoUpload)
    print generateInChannelPermissionKey(appID, appCertificate, channelname, unixts, randomint, uid, expiredts, AudioVideoUpload)

```

### Ruby
```ruby
require '../src/dynamic_key5'
app_id = "970ca35de60c44645bbae8a215061b33"
app_certificate = "5cfd2fd1755d40ecb72977518be15d3b"
channel_name = "7d72365eb983485397e3e3f9d460bdda"
unix_ts = Time.now.utc.to_i
uid = 2882341273
random_int = -2147483647
expired_ts = 0

puts "%.8x" % (random_int & 0xFFFFFFFF)

recording_key = DynamicKey5.gen_recording_key(app_id, app_certificate, channel_name, unix_ts, random_int, uid, expired_ts)
media_channel_key = DynamicKey5.gen_media_channel_key(app_id, app_certificate, channel_name, unix_ts, random_int, uid, expired_ts)
public_sharing_key = DynamicKey5.gen_public_sharing_key(app_id, app_certificate, channel_name, unix_ts, random_int, uid, expired_ts)
in_channel_permission_key1 = DynamicKey5.gen_in_channel_permission_key(app_id, app_certificate, channel_name, unix_ts, random_int, uid, expired_ts, DynamicKey5::NO_UPLOAD)
in_channel_permission_key2 = DynamicKey5.gen_in_channel_permission_key(app_id, app_certificate, channel_name, unix_ts, random_int, uid, expired_ts, DynamicKey5::AUDIO_VIDEO_UPLOAD)

puts "recording_key:#{recording_key}"
puts "media_channel_key:#{media_channel_key}"
puts "public_sharing_key:#{public_sharing_key}"
puts "in_channel_permission_key1:#{in_channel_permission_key1}"
puts "in_channel_permission_key2:#{in_channel_permission_key2}"

```

