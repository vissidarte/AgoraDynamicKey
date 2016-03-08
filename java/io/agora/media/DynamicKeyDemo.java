package io.agora.media;

/**
 * Demo of usage of DynamicKeyUtil.
 * Created by hefeng on 15/8/10.
 */
public class DynamicKeyDemo {
    public static void main(String[] args) throws Exception{
        DynamicKeyDemo demo = new DynamicKeyDemo();

        demo.demo1();
    }


    private void demo1() throws Exception{
        String vkey = "1";
        String signKey = "2";
        String cName = "3";
        int ts = 4;
        int rint = 5;

        try {
            String res = DynamicKey.generateDynamicKey(vkey, signKey, cName, ts, rint);

            System.out.println("Dyanmic key to join channel is:"+res);
        }catch (Exception  e){
            System.err.println("Failed to generate dynamic key");
            e.printStackTrace();
        }
    }
}
