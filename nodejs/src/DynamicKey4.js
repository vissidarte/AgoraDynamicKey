/**
* Server side. Generate Client key.
*/

var crypto = require('crypto');

module.exports.generateDynamicKey4 = function(vendorKey, signKey, channelName, unixTs, randomInt, uid, expiredTs, type) {
    uid=(new Uint32Array([uid]))[0]
    var version = "004";
    var unixTsStr = unixTs.toString();  //Unix Time stamp, track time as a running total of seconds
    var rndTxt = randomInt.toString(16);
    var randomIntStr = ("00000000" + rndTxt).substring(rndTxt.length);
    var expiredTsStr = ("0000000000" + expiredTs).substring(String(expiredTs).length);
    var uidStr = ("0000000000" + uid).substring(String(uid).length);
    if(type == 0){
        var sign = generateSignature4(vendorKey, signKey, channelName, unixTsStr, randomIntStr, uidStr, expiredTsStr);
    }else if(type == 1){
        var sign = generateRecordSignature(vendorKey, signKey, channelName, unixTsStr, randomIntStr, uidStr, expiredTsStr);
    }else{
        return "";
    }
    return version + sign + vendorKey + unixTsStr + randomIntStr+ expiredTsStr;
};

var generateSignature4 = function(vendorKey, signKey, channelName, unixTsStr, randomIntStr,uidStr, expiredTsStr ) {
    var buffer = Buffer.concat([new Buffer('ACS'), new Buffer(vendorKey), new Buffer(unixTsStr), new Buffer(randomIntStr), new Buffer(channelName), new Buffer(uidStr), new Buffer(expiredTsStr)]);
    var sign = encodeHMac(signKey, buffer);
    return sign.toString('hex');
};

var generateRecordSignature = function(vendorKey, signKey, channelName, unixTsStr, randomIntStr, uidStr, expiredTsStr ) {
    var buffer = Buffer.concat([new Buffer('ARS'),new Buffer(vendorKey), new Buffer(unixTsStr), new Buffer(randomIntStr), new Buffer(channelName), new Buffer(uidStr), new Buffer(expiredTsStr)]);
    var sign = encodeHMac(signKey, buffer);
    return sign.toString('hex');
};

var encodeHMac = function(key, message) {
    return crypto.createHmac('sha1', key).update(message).digest('hex');
};
