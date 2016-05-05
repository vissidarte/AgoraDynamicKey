#!/bin/bash
g++ DynamicKey_test.cpp -lcrypto -std=c++0x -lgtest -lpthread -o DynamicKey_test.exe
g++ DynamicKey3_test.cpp -lcrypto -std=c++0x -lgtest -lpthread -o DynamicKey_test3.exe
g++ DynamicKey4_test.cpp -lcrypto -std=c++0x -lgtest -lpthread -o DynamicKey_test4.exe
