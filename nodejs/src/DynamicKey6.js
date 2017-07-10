var crypto = require('crypto');

var version = "006";
var noUpload = "0";
var audioVideoUpload = "3";

var generatePublicSharingKey = function (appID, appCertificate, channelName) {
    return generatePublicSharingKeyFull(appID, appCertificate, channelName, getTimestamp(), getSalt(), 0, 0);
};

var generatePublicSharingKeyFull = function (appID, appCertificate, channelName, unixTs, salt, uid, expiredTs) {
    channelName=channelName.toString();
    return generateDynamicKey(appID, appCertificate, channelName, unixTs, salt, uid, expiredTs, null, PUBLIC_SHARING_SERVICE);
};

var generateRecordingKey = function (appID, appCertificate, channelName, uid) {
    return generateRecordingKeyFull(appID, appCertificate, channelName, getTimestamp(), getSalt(), uid, 0);
};

var generateRecordingKeyFull = function (appID, appCertificate, channelName, unixTs, salt, uid, expiredTs) {
    channelName=channelName.toString();
    return generateDynamicKey(appID, appCertificate, channelName, unixTs, salt, uid, expiredTs, null, RECORDING_SERVICE);
};

var generateMediaChannelKey = function (appID, appCertificate, channelName) {
    return generateMediaChannelKeyUid(appID, appCertificate, channelName, 0);
};

var generateMediaChannelKeyUid = function (appID, appCertificate, channelName, uid) {
    return generateMediaChannelKeyUidKickTime(appID, appCertificate, channelName, uid, 0);
};

var generateMediaChannelKeyUidKickTime = function (appID, appCertificate, channelName, uid, kickTs) {
    return generateMediaChannelKeyFull(appID, appCertificate, channelName, getTimestamp(), getSalt(), uid, kickTs);
};

var generateMediaChannelKeyFull = function (appID, appCertificate, channelName, unixTs, salt, uid, expiredTs) {
    channelName=channelName.toString();
    return generateDynamicKey(appID, appCertificate, channelName, unixTs, salt, uid, expiredTs, null, MEDIA_CHANNEL_SERVICE);
};

var generateInChannelPermissionKey = function (appID, appCertificate, channelName, uid, permission) {
    return generateInChannelPermissionKeyFull(appID, appCertificate, channelName, getTimestamp(), getSalt(), uid, 0, permission);
};

var generateInChannelPermissionKeyFull = function (appID, appCertificate, channelName, unixTs, salt, uid, expiredTs, permission) {
    var extra = {};
    extra[ALLOW_UPLOAD_IN_CHANNEL] = permission;
    return generateDynamicKey(appID, appCertificate, channelName, unixTs, salt, uid, expiredTs, extra, IN_CHANNEL_PERMISSION);
};

var generateDynamicKey = function (appID, appCertificate, channelName, unixTs, salt, uid, expiredTs, extra, serviceType) {
    var signature = generateSignature6(appCertificate, serviceType, appID, unixTs, salt, channelName, uid, expiredTs, extra);
    var content = DynamicKey6Content({
        serviceType: serviceType
        , signature: signature
        , uid: uid
        , unixTs: unixTs
        , salt: salt
        , expiredTs: expiredTs
        , extra: extra}).pack();
    return version + appID + content.toString('base64');
};

var getTimestamp = function() {
    return Math.floor(Date.now() / 1000);
};

var getSalt = function() {
    return crypto.randomBytes(4).readUInt32LE(0);
};

var fromString = function(keyString) {
    var that = {
        parsed: false
        , signature: ""
        , appID: ""
        , uid: 0
        , unixTs: 0
        , salt: 0
        , expiredTs: 0
        , extra: {}
    };

    if (keyString.substr(0, VERSION_LENGTH) != version) {
        return that;
    }

    var appID = keyString.substr(VERSION_LENGTH, APP_ID_LENGTH);
    if (appID.length != APP_ID_LENGTH) {
        return that;
    }
    that.appID = appID;

    var rawContent = Buffer.from(keyString.substr(VERSION_LENGTH + APP_ID_LENGTH), 'base64');
    if (rawContent.length == 0) {
        return that;
    }

    var content = DynamicKey6Content().unpack(rawContent);

    that.signature = content.signature;
    that.uid = content.uid;
    that.unixTs = content.unixTs;
    that.salt = content.salt;
    that.expiredTs = content.expiredTs;
    that.extra = content.extra;

    that.parsed = true;

    return that;
};

var generateSignature6 = function(appCertificate, serviceType, appID, unixTs, salt, channelName, uid, expiredTs, extra) {
    // decode hex to avoid case problem
    var rawAppID = hexDecode(appID);
    var rawAppCertificate = hexDecode(appCertificate);

    var m = Message({
        serviceType: serviceType
        , appID: rawAppID
        , unixTs: unixTs
        , salt: salt
        , channelName: channelName
        , uid: uid
        , expiredTs: expiredTs
        , extra: extra
    });

    var toSign = m.pack();
    return encodeHMac(rawAppCertificate, toSign);
};

var encodeHMac = function(key, message) {
    return crypto.createHmac('sha1', key).update(message).digest('hex').toUpperCase();
};

var hexDecode = function(str) {
    return new Buffer(str, 'hex');
};

var ByteBuf = function() {
    var that = {
        buffer: new Buffer(1024)
        , position: 0
    };

    that.buffer.fill(0);

    that.pack = function() {
        var out = new Buffer(that.position);
        that.buffer.copy(out, 0, 0, out.length);
        return out;
    };

    that.putUint16 = function(v) {
        that.buffer.writeUInt16LE(v, that.position);
        that.position += 2;
        return that;
    };

    that.putUint32 = function(v) {
        that.buffer.writeUInt32LE(v, that.position);
        that.position += 4;
        return that;
    };

    that.putBytes = function(bytes) {
        that.putUint16(bytes.length);
        bytes.copy(that.buffer, that.position);
        that.position += bytes.length;
        return that;
    };

    that.putString = function(str) {
        return that.putBytes(new Buffer(str));
    };

    that.putTreeMap = function(map) {
        if (!map) {
            that.putUint16(0);
            return that;
        }

        that.putUint16(Object.keys(map).length);
        for (var key in map) {
            that.putUint16(key);
            that.putString(map[key]);
        }

        return that;
    };

    that.getUint16 = function() {
        var v = that.buffer.readUInt16LE(that.position);
        that.position += 2;
        return v;
    };

    that.getUint32 = function() {
        var v = that.buffer.readUInt32LE(that.position);
        that.position += 4;
        return v;
    };

    that.getString = function() {
        var length = that.getUint16();
        var str = that.buffer.toString('ascii', that.position, length + that.position);
        that.position += length;
        return str;
    };

    that.getTreeMap = function() {
        var m = {};

        var count = that.getUint16();
        for (var i = 0; i < count; ++i) {
            var k = that.getUint16();
            var v = that.getString();
            m[k] = v;
        }

        return m;
    };

    return that;
}

var DynamicKey6Content = function(options) {
    var that = {};
    that.pack = function() {
        var out = ByteBuf();
        return out.putUint16(options.serviceType)
            .putString(options.signature)
            .putUint32(options.uid)
            .putUint32(options.unixTs)
            .putUint32(options.salt)
            .putUint32(options.expiredTs)
            .putTreeMap(options.extra)
            .pack();
    };

    that.unpack = function(buffer) {
        var buf = ByteBuf();
        buf.buffer = buffer;

        var content = {};
        content.serviceType = buf.getUint16();
        content.signature = buf.getString();
        content.uid = buf.getUint32();
        content.unixTs = buf.getUint32();
        content.salt = buf.getUint32();
        content.expiredTs = buf.getUint32();
        content.extra = buf.getTreeMap();
        return content;
    };

    return that;
}

var Message = function(options) {
    options.pack = function() {
        var out = ByteBuf();
        return out.putUint16(options.serviceType)
            .putBytes(options.appID)
            .putUint32(options.unixTs)
            .putUint32(options.salt)
            .putString(options.channelName)
            .putUint32(options.uid)
            .putUint32(options.expiredTs)
            .putTreeMap(options.extra)
            .pack();
    }

    return options;
}

// InChannelPermissionKey
var ALLOW_UPLOAD_IN_CHANNEL = 1;

// Service Type
var MEDIA_CHANNEL_SERVICE = 1;
var RECORDING_SERVICE = 2;
var PUBLIC_SHARING_SERVICE = 3;
var IN_CHANNEL_PERMISSION = 4;

// length
var VERSION_LENGTH = 3;
var APP_ID_LENGTH = 32;

module.exports.version = version;
module.exports.ALLOW_UPLOAD_IN_CHANNEL = ALLOW_UPLOAD_IN_CHANNEL;
module.exports.noUpload = noUpload;
module.exports.audioVideoUpload = audioVideoUpload;
module.exports.generatePublicSharingKey = generatePublicSharingKey;
module.exports.generatePublicSharingKeyFull = generatePublicSharingKeyFull;
module.exports.generateRecordingKey = generateRecordingKey;
module.exports.generateRecordingKeyFull = generateRecordingKeyFull;
module.exports.generateMediaChannelKey = generateMediaChannelKey;
module.exports.generateMediaChannelKeyUid = generateMediaChannelKeyUid;
module.exports.generateMediaChannelKeyUidKickTime = generateMediaChannelKeyUidKickTime;
module.exports.generateMediaChannelKeyFull = generateMediaChannelKeyFull;
module.exports.generateInChannelPermissionKey = generateInChannelPermissionKey;
module.exports.generateInChannelPermissionKeyFull = generateInChannelPermissionKeyFull;
module.exports.generateDynamicKey = generateDynamicKey;
module.exports.generateSignature = generateSignature6;
module.exports.fromString = fromString;
module.exports.getTimestamp = getTimestamp;
module.exports.MEDIA_CHANNEL_SERVICE = MEDIA_CHANNEL_SERVICE;
module.exports.RECORDING_SERVICE = RECORDING_SERVICE;
module.exports.PUBLIC_SHARING_SERVICE = PUBLIC_SHARING_SERVICE;
module.exports.IN_CHANNEL_PERMISSION = IN_CHANNEL_PERMISSION;
