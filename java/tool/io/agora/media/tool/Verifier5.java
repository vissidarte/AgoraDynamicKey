package io.agora.media.tool;

import io.agora.media.DynamicKey5;
import org.apache.commons.codec.binary.Hex;

/**
 * Created by liwei on 8/2/17.
 */
public class Verifier5 {
    public static void main(String[] args) throws Exception {
        if (args.length < 5) {
            System.out.println("java io.agora.media.tool.Verifier5 appID appCertificate channelName uid channelKey");
            return;
        }

        String appID = args[0];
        String appCertificate = args[1];
        String channelName = args[2];
        int uid = Integer.parseInt(args[3]);
        String channelKey = args[4];

        DynamicKey5 key5 = new DynamicKey5();
        if (! key5.fromString(channelKey)) {
            System.out.println("Faile to parse key");
            return ;
        }

        System.out.println("signature " + key5.content.signature);
        System.out.println("appID " + new String(Hex.encodeHex(key5.content.appID, false)));
        System.out.println("unixTs " + key5.content.unixTs);
        System.out.println("randomInt " + key5.content.salt);
        System.out.println("expiredTs " + key5.content.expiredTs);
        System.out.println("extra " + key5.content.extra.size());
        System.out.println("service " + key5.content.serviceType);

        System.out.println();
        System.out.println("Original \t\t " + channelKey);
        System.out.println("Uid = 0 \t\t " + DynamicKey5.generateMediaChannelKey(appID, appCertificate, channelName, key5.content.unixTs, key5.content.salt, 0, key5.content.expiredTs));
        System.out.println("Uid =  " + uid + " \t " + DynamicKey5.generateMediaChannelKey(appID, appCertificate, channelName, key5.content.unixTs, key5.content.salt, uid, key5.content.expiredTs));

        String signature = DynamicKey5.generateSignature(appCertificate,
                key5.content.serviceType,
                appID,
                key5.content.unixTs,
                key5.content.salt,
                channelName,
                uid,
                0,
                key5.content.extra
        );
        System.out.println("generated signature " + signature);
    }
}
