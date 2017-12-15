import java.security.NoSuchAlgorithmException;

public class SignalingTokenTest {

    public void testSignalingToken() throws NoSuchAlgorithmException {

        String appId = "970ca35de60c44645bbae8a215061b33";
        String appCertificate = "5cfd2fd1755d40ecb72977518be15d3b";
        String account = "TestAccount";
        int erpired = 1446455471;
        String result = SignalingToken.getToken(appId, appCertificate, account, erpired);
        String expected = "1:970ca35de60c44645bbae8a215061b33:-1335259689:c31216ea77c6461c8138e184387611b6";
        
    }
}
