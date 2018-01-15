package io.agora.media.tool;

import io.agora.media.AccessToken;

import java.util.Date;
import java.util.Random;

public class AccessTokenSample {
    private String appId = "970CA35de60c44645bbae8a215061b33";
    private String appCertificate = "5CFd2fd1755d40ecb72977518be15d3b";
    private String channelName = "7d72365eb983485397e3e3f9d460bdda";
    private String uid = "2882341273";
    private int ts = (int) (new Date().getTime() / 1000);
    private int salt = new Random().nextInt();
    private int expiredTs = 0;

    public void testGenerateDynamicKey() throws Exception {
        String expected = "006970CA35de60c44645bbae8a215061b33IACV0fZUBw+72cVoL9eyGGh3Q6Poi8bgjwVLnyKSJyOXR7dIfRBXoFHlEAABAAAAR/QQAAEAAQCvKDdW";
        AccessToken token = new AccessToken(appId, appCertificate, channelName, uid, ts, salt);
        token.addPrivilege(AccessToken.Privileges.kJoinChannel, expiredTs);
        String result = token.build();
        System.out.println(result);
    }
}
