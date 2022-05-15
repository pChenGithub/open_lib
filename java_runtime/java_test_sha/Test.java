package java_runtime.java_test_sha;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.GregorianCalendar;
import java.util.Map;
import java.util.TreeMap;
import java_runtime.java_test_sha.*;

public class Test {
    
    private static String toDate(long milliscond) {
        Date date = new Date(milliscond);
        GregorianCalendar gc = new GregorianCalendar();
        gc.setTime(date);
        SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        String data = simpleDateFormat.format(gc.getTime());
        return data;
    }


    public static void main(String[] args) {
/*
        Map<String, String> strmap = new TreeMap<>();

        strmap.put("method", "hzsun.face.version");
        strmap.put("format", "json");
        strmap.put("charset", "utf-8");
        strmap.put("publickey", "123456");
        strmap.put("sign_type", "RSA2");
        //strmap.put("timestamp", toDate(System.currentTimeMillis()));
        strmap.put("timestamp", "2021-06-07 15:33:42");
        strmap.put("version", "1.0");
        strmap.put("biz_content", "{\"epid\":\"0\",\"device_sn\":\"cc.4b.73.bd.1e.ac\"}");

        String content = strmap.toString().replace(", ", "&");
        content = content.substring(1, content.length()-1);

        System.out.println(content);
*/
        //System.out.println("hello word");

        //String strout = MyUtils.encrypt(content, 0);
        //System.out.println(strout);


        byte[] raw;
        //String strout = "biz_content={\"epid\":\"0\",\"device_sn\":\"cc.4b.73.bd.1e.ac\"}&charset=utf-8&format=json&method=hzsun.face.version&publickey=123456&sign_type=RSA2&timestamp=2021-06-07 15:33:42&version=1.0";
        //String strout = "biz_content={\"epid\":\"0\",\"device_sn\":\"cc.4b.73.bd.1e.ac\",\"version\"=\"28265\"}&charset=utf-8&format=json&method=hzsun.facepic.initdata&publickey=123456&sign_type=SHA&timestamp=2021-06-07 15:33:42&version=1.0";
        //String strout = "biz_content={\"epid\":\"0\",\"device_sn\":\"cc.4b.73.bd.1e.ac\"}&charset=utf-8&format=json&method=hzsun.face.version&publickey=123456&sign_type=RSA2&timestamp=1970-01-02 05:14:29&version=1.0";
        // String strout = "Method=getCardsChangeInfo&biz_content={\"protocolVersion\":\"1\",\"serialNum\":\"cc4b73bd1eac\",\"deviceCardsChangeFixid\":\"0\"}&publickey=123456";
        String strout = "biz_content={\"Timegroupversion\":\"0\",\"Sysversion\":\"18\",\"FireStatus\":\"0\",\"Timetempletversion\":\"2\",\"Accrightversion\":\"4040\",\"Devicetype\":\"1\",\"Accversion\":\"2026\",\"Doorholidayversion\":\"6\",\"Stanum\":\"3\",\"Staip\":\"d41243627940\",\"Terversion\":\"7\",\"Uneventcnt\":\"0\",\"Doorstatus\":\"1\",\"Doorversion\":\"20\"}&charset=utf-8&format=json&method=VersionInfo&publickey=123456&sign_type=RSA2&timestamp=2021-12-14 14:01:19&version=1.0";
/** 
        raw = content.getBytes();
        for (int i=0;i<raw.length;i++) {
            System.out.printf("%x ", raw[i]);
        }
        System.out.printf("\n\n\n");
        
*/

        System.out.println(strout);
        raw = Sha1Utils.shaEncode(strout); // 1
        System.out.println(MyUtils.stringToHexString(raw.toString()));
        for (int i=0;i<raw.length;i++) {
            System.out.printf("%d ", raw[i]);
        }
        System.out.printf("\n\n\n");

        strout = Base64Utils.encode(raw); // 这个跟在线工具一致
        System.out.println(strout);

        strout = UrlUtils.encode(strout);; // 这个跟在线工具一致
        System.out.println(strout);
    }
}
