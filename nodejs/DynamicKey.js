/**
* Server side. Generate Client key.
*/

var crypto = require('crypto');

var generateDynamicKey = function(vendorKey, signKey, channelName, unixTs, randomInt) {
    var unixTsStr = ("0000000000" + unixTs).substring(String(unixTs).length);
    var rndTxt = randomInt.toString(16);
    var randomIntStr = ("00000000" + rndTxt).substring(rndTxt.length);
    var sign = generateSignature(vendorKey, signKey, channelName, unixTsStr, randomIntStr);
    return sign + vendorKey + unixTsStr + randomIntStr;
};

var generateDynamicKey2 = function(vendorKey, signKey, channelName, unixTs, randomInt, uid, expiredTs) {
    var unixTsStr = unixTs.toString();  //Unix Time stamp, track time as a running total of seconds
    var rndTxt = randomInt.toString(16);
    var randomIntStr = ("00000000" + rndTxt).substring(rndTxt.length);
    var uidStr = ("0000000000" + uid).substring(String(uid).length);
    var expiredTsStr = ("0000000000" + expiredTs).substring(String(expiredTs).length);
    var sign = generateSignature2(vendorKey, signKey, channelName, unixTsStr, randomIntStr, uidStr, expiredTsStr);
    return sign + vendorKey + unixTsStr + randomIntStr+ uidStr+ expiredTsStr;
};

var generateDynamicKey3 = function(vendorKey, signKey, channelName, unixTs, randomInt, uid, expiredTs) {
    var version = "003";
    var unixTsStr = unixTs.toString();  //Unix Time stamp, track time as a running total of seconds
    var rndTxt = randomInt.toString(16);
    var randomIntStr = ("00000000" + rndTxt).substring(rndTxt.length);
    var uidStr = ("0000000000" + uid).substring(String(uid).length);
    var expiredTsStr = ("0000000000" + expiredTs).substring(String(expiredTs).length);
    var sign = generateSignature2(vendorKey, signKey, channelName, unixTsStr, randomIntStr, uidStr, expiredTsStr);
    return version + sign + vendorKey + unixTsStr + randomIntStr+ uidStr+ expiredTsStr;
};


var generateSignature = function(vendorKey, signKey, channelName, unixTsStr, randomIntStr) {
    var buffer = Buffer.concat([new Buffer(vendorKey), new Buffer(unixTsStr), new Buffer(randomIntStr), new Buffer(channelName)]);
    var sign = encodeHMac(signKey, buffer);
    return sign.toString('hex');
};

var generateSignature2 = function(vendorKey, signKey, channelName, unixTsStr, randomIntStr,uidStr, expiredTsStr ) {
    var buffer = Buffer.concat([new Buffer(vendorKey), new Buffer(unixTsStr), new Buffer(randomIntStr), new Buffer(channelName), new Buffer(uidStr), new Buffer(expiredTsStr)]);
    var sign = encodeHMac(signKey, buffer);
    return sign.toString('hex');
};


var encodeHMac = function(key, message) {
    return crypto.createHmac('sha1', key).update(message).digest('hex');
};

var vendor = "970ca35de60c44645bbae84215061b33b";
var key = "5cfd2fd1755d40ecb72977518be15d3b";
var channel = "7d72365eb983485397e3e3f9d460bdda";
var ts = 1446455472;
var r = 58964981;
var uid=999;
var expiredTs=1446455471;

console.log(generateDynamicKey(vendor, key, channel, ts, r));
console.log(generateDynamicKey2(vendor, key, channel, ts, r, uid, expiredTs));
console.log(generateDynamicKey3(vendor, key, channel, ts, r, uid, expiredTs));
