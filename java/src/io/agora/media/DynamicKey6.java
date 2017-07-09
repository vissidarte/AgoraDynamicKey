package io.agora.media;

import org.apache.commons.codec.binary.Base64;
import org.apache.commons.codec.binary.Hex;

import java.util.Date;
import java.util.Random;
import java.util.TreeMap;

/**
 * Created by liwei on 7/9/17.
 */
public class DynamicKey6 {
    public final static String version = "006";
    public final static String noUpload = "0";
    public final static String audioVideoUpload = "3";

    // ServiceType
    public final static short MEDIA_CHANNEL_SERVICE = 1;
    public final static short RECORDING_SERVICE = 2;
    public final static short PUBLIC_SHARING_SERVICE = 3;
    public final static short IN_CHANNEL_PERMISSION = 4;

    // InChannelPermissionKey
    public final static short ALLOW_UPLOAD_IN_CHANNEL = 1;

    private static final int VERSION_LENGTH = 3;
    private static final int APP_ID_LENGTH = 32;

    static String generateSignature(String appCertificate, short service, String appID, int unixTs, int salt, String channelName, long uid, int expiredTs, TreeMap<Short, String> extra) throws Exception {
        // decode hex to avoid case problem
        Hex hex = new Hex();
        byte[] rawAppID = hex.decode(appID.getBytes());
        byte[] rawAppCertificate = hex.decode(appCertificate.getBytes());

        Message m = new Message(service, rawAppID, unixTs, salt, channelName, (int)(uid & 0xFFFFFFFFL), expiredTs, extra);
        byte[] toSign = pack(m);
        return new String(Hex.encodeHex(DynamicKeyUtil.encodeHMAC(rawAppCertificate, toSign), false));
    }

    public static String generateDynamicKey(String appID, String appCertificate, String channel, int ts, int salt, long uid, int expiredTs, TreeMap<Short, String> extra, short service) throws Exception {
        String signature = generateSignature(appCertificate, service, appID, ts, salt, channel, uid, expiredTs, extra);
        DynamicKey6Content content = new DynamicKey6Content(service, signature, (int)uid, ts, salt, expiredTs, extra);
        byte[] bytes = pack(content);
        byte[] encoded = new Base64().encode(bytes);
        String base64 = new String(encoded);
        return version + appID + base64;
    }

    private static byte[] pack(Packable content) {
        ByteBuf buffer = new ByteBuf();
        content.marshall(buffer);
        return buffer.asBytes();
    }

    public static String generatePublicSharingKey(String appID, String appCertificate, String channel, int ts, int salt, long uid, int expiredTs) throws Exception {
        return generateDynamicKey(appID, appCertificate, channel, ts, salt, uid, expiredTs, new TreeMap<Short, String>(), PUBLIC_SHARING_SERVICE);
    }

    public static String generateRecordingKey(String appID, String appCertificate, String channel, int ts, int salt, long uid, int expiredTs) throws Exception {
        return generateDynamicKey(appID, appCertificate, channel, ts, salt, uid, expiredTs, new TreeMap<Short, String>(), RECORDING_SERVICE);
    }

    public static String generateMediaChannelKey(String appID, String appCertificate, String channel) throws Exception {
        return generateMediaChannelKey(appID, appCertificate, channel, 0);
    }

    public static String generateMediaChannelKey(String appID, String appCertificate, String channel, long uid) throws Exception {
        return generateMediaChannelKey(appID, appCertificate, channel, uid, 0);
    }

    public static String generateMediaChannelKey(String appID, String appCertificate, String channel, long uid, int expiredTs) throws Exception {
        return generateMediaChannelKey(appID, appCertificate, channel, getTimestamp(), getSalt(), uid, expiredTs);
    }

    public static String generateMediaChannelKey(String appID, String appCertificate, String channel, int ts, int salt, long uid, int expiredTs) throws Exception {
        return generateDynamicKey(appID, appCertificate, channel, ts, salt, uid, expiredTs, new TreeMap<Short, String>(), MEDIA_CHANNEL_SERVICE);
    }

    public static String generateInChannelPermissionKey(String appID, String appCertificate, String channel, long uid, String permission) throws Exception {
        return generateInChannelPermissionKey(appID, appCertificate, channel, getTimestamp(), getSalt(), uid, 0, permission);
    }

    public static String generateInChannelPermissionKey(String appID, String appCertificate, String channel, int ts, int salt, long uid, int expiredTs, String permission) throws Exception {
        TreeMap<Short, String> extra = new TreeMap<Short, String>();
        extra.put(ALLOW_UPLOAD_IN_CHANNEL, permission);
        return generateDynamicKey(appID, appCertificate, channel, ts, salt, uid, expiredTs, extra, IN_CHANNEL_PERMISSION);
    }

    String signature = "";
    String appID = "";
    long uid;
    int unixTs ;
    int salt;
    int expiredTs;
    TreeMap<Short, String> extra = new TreeMap<Short, String>();

    static int getTimestamp() {
        return (int)(new Date().getTime()/1000);
    }

    private static int getSalt() {
        return new Random(new Date().getTime()).nextInt();
    }

    public boolean fromString(String channelKeyString) {
        if (!version.equals(channelKeyString.substring(0, VERSION_LENGTH))) {
            return false;
        }

        appID = channelKeyString.substring(VERSION_LENGTH, VERSION_LENGTH + APP_ID_LENGTH);
        if (appID.length() != APP_ID_LENGTH) {
            return false;
        }

        byte[] rawContent = new Base64().decode(channelKeyString.substring(VERSION_LENGTH + APP_ID_LENGTH));
        if (rawContent.length == 0) {
            return false;
        }

        DynamicKey6Content content = new DynamicKey6Content(rawContent);
        signature = content.signature;
        uid = content.uid & 0xFFFFFFFFL;
        unixTs = content.unixTs;
        salt = content.salt;
        expiredTs = content.expiredTs;
        extra = content.extra;
        return true;
    }

    static class Message implements Packable {
        public short serviceType;
        public byte[] appID;
        public int unixTs;
        public int salt;
        public String channelName;
        public int uid;
        public int expiredTs;
        public TreeMap<Short, String> extra;

        public Message(short serviceType, byte[] appID, int unixTs, int salt, String channelName, int uid, int expiredTs, TreeMap<Short, String> extra) {
            this.serviceType = serviceType;
            this.appID = appID;
            this.unixTs = unixTs;
            this.salt = salt;
            this.channelName = channelName;
            this.uid = uid;
            this.expiredTs = expiredTs;
            this.extra = extra;
        }

        public ByteBuf marshall(ByteBuf out) {
            return out.put(serviceType).put(appID).put(unixTs).put(salt).put(channelName).put(uid).put(expiredTs).put(extra);
        }
    }

    static class DynamicKey6Content implements Packable {
        public short serviceType;
        public String signature = "";
        public int uid;
        public int unixTs;
        public int salt;
        public int expiredTs;
        public TreeMap<Short, String> extra = new TreeMap<Short, String>();

        public DynamicKey6Content(short serviceType, String signature, int uid, int unixTs, int salt, int expiredTs, TreeMap<Short, String> extra) {
            this.serviceType = serviceType;
            this.signature = signature;
            this.uid = uid;
            this.unixTs = unixTs;
            this.salt = salt;
            this.expiredTs = expiredTs;
            this.extra = extra;
        }

        public DynamicKey6Content(byte[] bytes) {
            unmarshall(new ByteBuf(bytes));
        }

        public ByteBuf marshall(ByteBuf out) {
            return out.put(serviceType).put(signature).put(uid).put(unixTs).put(salt).put(expiredTs).put(extra);
        }

        public void unmarshall(ByteBuf in) {
            this.serviceType = in.readShort();
            this.signature = in.readString();
            this.uid = in.readInt();
            this.unixTs = in.readInt();
            this.salt = in.readInt();
            this.expiredTs = in.readInt();
            in.readMap(this.extra);
        }
    }}
