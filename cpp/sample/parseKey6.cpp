/**
 * build with command:
 * g++ parseKey6.cpp  -lcrypto -std=c++0x
 */
#include "../src/DynamicKey6.h"
#include <iostream>
#include <cstdint>
using namespace agora::tools;

template<typename T>
inline std::string map_to_string(const T & c, const std::string& separator)
{
    std::ostringstream oss;
    std::string s = "";
    for (const auto & v: c) {
        oss << s << v.first << ":" << v.second;
        s = separator;
    }

    return oss.str();
}

int main(int argc, char const *argv[]) {
    std::string key = argv[1];

    DynamicKey6 k6;
    if (! k6.fromString(key)) {
        return 0;
    }

    std::cout << "App ID:     " << k6.appID << std::endl;
    std::cout << "UID:        " << k6.uid << std::endl;
    std::cout << "Timestamp:  " << k6.unixTs << std::endl;
    std::cout << "Expired:    " << k6.expiredTs << std::endl;
    std::cout << "Extra:      [" << map_to_string(k6.extra, ", ") << "]" << std::endl;
    std::cout << "Salt:       " << k6.salt << std::endl;
    std::cout << "Signature:  " << k6.signature << std::endl;

    return 0;
}
