/**
* Server side. Generate Client key.
*/

var crypto = require('crypto');


module.exports.generate = function(vendorKey, signKey, channelName, unixTs, randomInt, uid, expiredTs) {
    var version = "003";
    var unixTsStr = unixTs.toString();  //Unix Time stamp, track time as a running total of seconds
    var rndTxt = randomInt.toString(16);
    var randomIntStr = ("00000000" + rndTxt).substring(rndTxt.length);
    var uidStr = ("0000000000" + uid).substring(String(uid).length);
    var expiredTsStr = ("0000000000" + expiredTs).substring(String(expiredTs).length);
    var sign = generateSignature3(vendorKey, signKey, channelName, unixTsStr, randomIntStr, uidStr, expiredTsStr);
    return version + sign + vendorKey + unixTsStr + randomIntStr+ uidStr+ expiredTsStr;
};

var generateSignature3 = function(vendorKey, signKey, channelName, unixTsStr, randomIntStr,uidStr, expiredTsStr ) {
    var buffer = Buffer.concat([new Buffer(vendorKey), new Buffer(unixTsStr), new Buffer(randomIntStr), new Buffer(channelName), new Buffer(uidStr), new Buffer(expiredTsStr)]);
    var sign = encodeHMac(signKey, buffer);
    return sign.toString('hex');
};


var encodeHMac = function(key, message) {
    return crypto.createHmac('sha1', key).update(message).digest('hex');
};
