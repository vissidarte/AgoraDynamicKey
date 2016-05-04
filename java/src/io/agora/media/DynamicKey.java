package io.agora.media;

import java.io.ByteArrayOutputStream;

/**
 * Created by hefeng on 15/8/10.
 * Util to generate Agora media dynamic key.
 */
public class DynamicKey {
    /**
     * Generate Dynamic Key for media channel service
     * @param vendorKey Vendor key assigned by Agora
     * @param signKey Sign key assigned by Agora
     * @param channelName name of channel to join, limited to 64 bytes and should be printable ASCII characters
     * @param unixTs unix timestamp in seconds when generating the Dynamic Key
     * @param randomInt salt for generating dynamic key
     * @return String representation of dynamic key
     * @throws Exception
     */
    public static String generate(String vendorKey, String signKey, String channelName, int unixTs, int randomInt) throws Exception {
        String unixTsStr = ("0000000000" + Integer.toString(unixTs)).substring(Integer.toString(unixTs).length());
        String randomIntStr = ("00000000" + Integer.toHexString(randomInt)).substring(Integer.toHexString(randomInt).length());
        String signature = generateSignature(vendorKey, signKey, channelName, unixTsStr, randomIntStr);
        return String.format("%s%s%s%s", signature, vendorKey, unixTsStr, randomIntStr);
    }

    private static String generateSignature(String vendorKey, String signKey, String channelName, String unixTsStr, String randomIntStr) throws Exception {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        baos.write(vendorKey.getBytes());
        baos.write(unixTsStr.getBytes());
        baos.write(randomIntStr.getBytes());
        baos.write(channelName.getBytes());
        byte[] sign = DynamicKeyUtil.encodeHMAC(signKey, baos.toByteArray());
        return DynamicKeyUtil.bytesToHex(sign);
    }

}
