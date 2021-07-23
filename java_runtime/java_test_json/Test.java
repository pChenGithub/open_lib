package open_lib.java_runtime.java_test_json;

import java.util.Map;
import java.util.TreeMap;

import org.json.JSONArray;
import org.json.JSONObject;

class Test {

    public static void main(String[] args) {
        // String src = "{\"code\":\"00000","msg":"SUCCESS","user_info":"[{\"accclassid\":\"3\",\"accname\":\"混合卡21\",\"accnum\":\"15599\",\"campusid\":\"1\",\"cardcode\":\"132171\",\"cardno\":\"1324049305\",\"epid\":\"20\",\"flag\":\"2\",\"personcode\":\"YT002\",\"pic_url\":\"http://172.16.6.108:28080/facefeature-web//photourl/20210610404198341702844728\",\"version\":\"6662\"}]","sign":"+F+97xf7SsLV/v1NWDiXZMuygME="}";

        Map<String, String> map = new TreeMap<>();
        map.put("code", "00000");
        map.put("user_info", "[{\"accclassid\":\"3\",\"accname\":\"混合卡21\",\"accnum\":\"15599\",\"campusid\":\"1\",\"cardcode\":\"132171\",\"cardno\":\"1324049305\",\"epid\":\"20\",\"flag\":\"2\",\"personcode\":\"YT002\",\"pic_url\":\"http://172.16.6.108:28080/facefeature-web//photourl/20210610404198341702844728\",\"version\":\"6662\"}]");

        String src = map.toString();

        System.out.println(src);
        JSONArray array = new JSONArray(src);
        
        System.out.println(array.length());
        JSONObject obj = array.getJSONObject(0);

        System.out.println(obj.optString("accclassid"));
    }

}