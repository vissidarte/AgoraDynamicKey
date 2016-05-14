/**
 * build with command:
 * g++ main.cpp  -lcrypto -std=c++0x
 */

#include "../src/DynamicKey4.h"
#include <iostream>
#include <cstdint>

void generateRecordingKey(){
    auto staticKey  = "970ca35de60c44645bbae8a215061b33";
    auto  signKey   = "5cfd2fd1755d40ecb72977518be15d3b";
    auto channelName= "my channel name for recording";
    auto  unixTs = ::time(NULL);
    int randomInt = (::rand()%256 << 24) + (::rand()%256 << 16) + (::rand()%256 << 8) + (::rand()%256);
    uint32_t uid = 2882341273u;
    auto  expiredTs = 0;
    std::string result = agora::tools::DynamicKey4::generateRecordingKey(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs);
    std::cout << result << std::endl;
}

void generateMediaKey(){
    auto staticKey  = "970ca35de60c44645bbae8a215061b33";
    auto  signKey   = "5cfd2fd1755d40ecb72977518be15d3b";
    auto channelName= "my channel name for media service";
    auto  unixTs = ::time(NULL);
    int randomInt = (::rand()%256 << 24) + (::rand()%256 << 16) + (::rand()%256 << 8) + (::rand()%256);
    uint32_t uid = 2882341273u;
    auto  expiredTs = 0;
    std::string result = agora::tools::DynamicKey4::generateRecordingKey(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs);
    std::cout << result << std::endl;
}

int main(int argc, char const *argv[]) {
  ::srand(::time(NULL));
  generateRecordingKey();
  generateMediaKey();
  return 0;
}
