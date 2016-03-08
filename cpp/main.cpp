#include <iostream>
#include <vector>
#include <string>
#include "dynamic_key.h"

using namespace std;

int main(int argc, char* argv[])
{
	string staticKey;
	string signKey;
	string channelName;
	uint32_t unixTs ;
	uint32_t randomInt;
	if (argc < 6) {
		cout << "\n\t Usage: ./dynamicKey.exe staticKey signKey channelName unixTs randomInt\n" << endl;
        staticKey = "970ca35de60c44645bbae84215061b33b";
        signKey = "5cfd2fd1755d40ecb72977518be15d3b";
        channelName= "7d72365eb983485397e3e3f9d460bdda";
        unixTs = 1446455472;
        randomInt = 58964981;
    }else{
        staticKey=argv[1];
        signKey=argv[2];
        channelName=argv[3];
        unixTs = (uint32_t)atoi(argv[4]);
        randomInt = (uint32_t)(atoi(argv[5]));
    }
    /*
        staticKey  静态key  网站上生成的    ；
        signkey  网站上生成的 ；
        channelName   频道名字 ；
        unixTs  发送的时间戳;
        randomInt   随机数
    */
	cout << "---------------------------------------------------------------------------" << endl;
	cout << "Input parameters:" << endl;
	cout << "\tstaticKey:\t" << staticKey << endl;
	cout << "\tsignKey:\t" << signKey << endl;
	cout << "\tchannelName:\t" << channelName << endl;
	cout << "\tunixTs:\t" << unixTs << endl;
	cout << "\trandomInt:\t" << randomInt << endl;

	string result = agora::tools::generateDynamicKey(staticKey, signKey, channelName, unixTs, randomInt);
	cout << "\nResult:" << endl;
	cout << "\t" << result << endl;
	cout << "---------------------------------------------------------------------------" << endl;

	return 0;
}
