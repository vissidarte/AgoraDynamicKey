/**
* Server side. Generate Client key.
*/

var crypto = require('crypto');

var encodeHMac = function(key, message) {
    return crypto.createHmac('sha1', key).update(message).digest('hex');
};

module.exports.generate = function(vendorKey, signKey, channelName, unixTs, randomInt) {
    var unixTsStr = ("0000000000" + unixTs).substring(String(unixTs).length);
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
