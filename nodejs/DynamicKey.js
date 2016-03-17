/**
* Server side. Generate Client key.
*/

var crypto = require('crypto');

var generateDynamicKey = function(vendorKey, signKey, channelName, unixTs, randomInt) {
    var unixTsStr = unixTs.toString();  //Unix Time stamp, track time as a running total of seconds
    var rndTxt = randomInt.toString(16);
    var randomIntStr = ("00000000" + rndTxt).substring(rndTxt.length);
    var sign = generateSignature(vendorKey, signKey, channelName, unixTsStr, randomIntStr);
    return sign + vendorKey + unixTsStr + randomIntStr;
};

var generateSignature = function(vendorKey, signKey, channelName, unixTsStr, randomIntStr) {
    var buffer = Buffer.concat([new Buffer(vendorKey), new Buffer(unixTsStr), new Buffer(randomIntStr), new Buffer(channelName)]);
    var sign = encodeHMac(signKey, buffer);
    return sign.toString('hex');
};

var encodeHMac = function(key, message) {
    return crypto.createHmac('sha1', key).update(message).digest('hex');
};

var vendor = "770ca35e60c44645bbae84215061b33b";
var key = "5cfd2f1755d40ecb72977518be15d3b";
var channel = "7d7236eb983485397e3e3f9d460bdda";
var ts = 1446455472;
var r = 58964981;

console.log(generateDynamicKey(vendor, key, channel, ts, r));
