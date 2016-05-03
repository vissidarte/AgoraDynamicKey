var DynamicKey = require('../src/DynamicKey');
var DynamicKey3 = require('../src/DynamicKey3');
var DynamicKey4 = require('../src/DynamicKey4');
var vendor = "970ca35de60c44645bbae84215061b33b";
var key = "5cfd2fd1755d40ecb72977518be15d3b";
var channel = "7d72365eb983485397e3e3f9d460bdda";
var ts = 1446455472;
var r = 58964981;
var uid=999;
var expiredTs=1446455471;

console.log("generateDynamicKey:");
console.log(DynamicKey.generateDynamicKey(vendor, key, channel, ts, r));
console.log("generateDynamicKey3:");
console.log(DynamicKey3.generateDynamicKey3(vendor, key, channel, ts, r, uid, expiredTs));
console.log("generateDynamicKey4:");
console.log(DynamicKey4.generateDynamicKey4(vendor, key, channel, ts, r, uid, expiredTs, 0));
console.log("generateRecordTicket:");
console.log(DynamicKey4.generateDynamicKey4(vendor, key, channel, ts, r, uid, expiredTs, 1));
