#include <iostream>
#include <vector>
#include <string>
#include "../src/DynamicKey.h"
#include "../src/DynamicKey3.h"
#include "../src/DynamicKey4.h"

using namespace std;

int main(int argc, char* argv[])
{
	string staticKey;
	string signKey;
	string channelName;
	uint32_t unixTs ;
	uint32_t randomInt;
    uint32_t uid;
    uint32_t expiredTs;
	if (argc < 6) {
		cout << "\n\t Usage: ./dynamicKey.exe staticKey signKey channelName unixTs randomInt\n" << endl;
        staticKey = "970ca35de60c44645bbae84215061b33b";
        signKey = "5cfd2fd1755d40ecb72977518be15d3b";
        channelName= "7d72365eb983485397e3e3f9d460bdda";
        unixTs = 1446455472;
        randomInt = 58964981;
        uid=-999;
        expiredTs=1446455471;
    }else{
        staticKey=argv[1];
        signKey=argv[2];
        channelName=argv[3];
        unixTs = (uint32_t)atoi(argv[4]);
        randomInt = (uint32_t)(atoi(argv[5]));
        unixTs = (uint32_t)atoi(argv[6]);
        unixTs = (uint32_t)atoi(argv[7]);
    }
    /*
        staticKey  静态key  网站上生成的    ；
        signkey  网站上生成的 ；
        channelName   频道名字 ；
        unixTs  发送的时间戳;
        randomInt   随机数
        uid 用户id
        expiredTs 通话有效时间戳
    */
	cout << "---------------------------------------------------------------------------" << endl;
	cout << "Input parameters:" << endl;
	cout << "\tstaticKey:\t" << staticKey << endl;
	cout << "\tsignKey:\t" << signKey << endl;
	cout << "\tchannelName:\t" << channelName << endl;
	cout << "\tunixTs:\t" << unixTs << endl;
	cout << "\trandomInt:\t" << randomInt << endl;
	cout << "\tuid:\t" << uid << endl;
	cout << "\texpiredTs:\t" << expiredTs << endl;

	string result = agora::tools::generateDynamicKey(staticKey, signKey, channelName, unixTs, randomInt);
	cout << "\ngenerateDynamicKey Result:" << endl;
	cout << "\t" << result << endl;
	cout << "---------------------------------------------------------------------------" << endl;

	result = agora::tools::generateDynamicKey3(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs);
	cout << "\ngenerateDynamicKey3 Result:" << endl;
	cout << "\t" << result << endl;
	cout << "---------------------------------------------------------------------------" << endl;

	result = agora::tools::generateDynamicKey4(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs, 0);
	cout << "\ngenerateDynamicKey4 Result:" << endl;
	cout << "\t" << result << endl;

	result = agora::tools::generateDynamicKey4(staticKey, signKey, channelName, unixTs, randomInt, uid, expiredTs, 1);
	cout << "\ngenerateRecordTicket Result:" << endl;
	cout << "\t" << result << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	return 0;
}
