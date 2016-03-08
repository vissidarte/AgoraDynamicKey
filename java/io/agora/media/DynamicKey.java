package io.agora.media;

import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;
import java.io.ByteArrayOutputStream;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;

/**
 * Created by hefeng on 15/8/10.
 * Util to generate Agora media dynamic key.
 */
public class DynamicKey {

    /**
     * Manipulate Agora dynamic key for media connection.
     * @param vendorKey Vendor key assigned by Agora when register
     * @param signKey Sign key assigned by Agora
     * @param channelName name of channel to join
     * @param unixTs unix timestamp by seconds
     * @param randomInt random uint32 salt for generating dynamic key
     * @return String representation of dynamic key to join Agora media server
     * @throws Exception if any error occurs
     */
    public static String generateDynamicKey(String vendorKey, String signKey, String channelName, int unixTs, int randomInt) throws Exception{
        String signature = signChannelInfo(vendorKey, signKey, channelName, unixTs, randomInt);
        return String.format("%s%s%010d%08d", signature, vendorKey, unixTs, randomInt);
    }

    private static String signChannelInfo(String vendorKey, String signKey, String channelName, int unixTs, int randomInt) throws Exception {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        baos.write(vendorKey.getBytes());
        baos.write(unixTs);
        baos.write(randomInt);
        baos.write(channelName.getBytes());

        byte[] sign = encodeHMAC(signKey, baos.toByteArray());
        return bytesToHex(sign);
    }

    private static byte[] encodeHMAC(String key, byte[] message) throws NoSuchAlgorithmException, InvalidKeyException {
        SecretKeySpec keySpec = new SecretKeySpec(
                key.getBytes(),
                "RAW");

        Mac mac = Mac.getInstance("HmacSHA1");
        mac.init(keySpec);
        return mac.doFinal(message);
    }

    private static String bytesToHex(byte[] in) {
        final StringBuilder builder = new StringBuilder();
        for(byte b : in) {
            builder.append(String.format("%02x", b));
        }
        return builder.toString();
    }
}
