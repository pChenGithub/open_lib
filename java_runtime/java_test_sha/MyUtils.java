package java_runtime.java_test_sha;

import java.net.URLEncoder;
import java.security.MessageDigest;
import java.util.Base64;

public class MyUtils {
    static public String stringToHexString(String source) {
        String ret = "";

        if ("".equals(source) || null==source)
            return null;

        for (int i=0;i<source.length();i++) {
            int ch = source.charAt(i);
            String s4 = Integer.toHexString(ch);
            ret = ret+s4;
        }

        return ret;
    }
	
	public static String encrypt(String password, int code) {
        MessageDigest md;
        try {
            md = MessageDigest.getInstance("SHA"); // 使用sha算法
            //Log.e("去生成签名",password);
            md.update(password.getBytes("UTF-8")); // step 3 // 指定password更新摘要
            byte raw[] = md.digest(); // step 4 // 完成hash计算


            // String hash = (new BASE64Encoder()).encode(raw); // step 5

            String hash = Base64.getEncoder().encodeToString(raw);

            //return hash; // step 6
            if (code == 1) {
                return hash; // step 6
            } else {
                return URLEncoder.encode(hash, "utf-8"); // step 6
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }
}
