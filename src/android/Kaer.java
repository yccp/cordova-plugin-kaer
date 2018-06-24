package news.chen.yu.ionic;

import android.content.Intent;
import android.graphics.Bitmap;
import android.os.AsyncTask;
import android.util.Base64;
import android.util.Log;

import org.apache.cordova.*;
import org.json.JSONException;
import org.json.JSONObject;

import com.kaer.sdk.IDCardItem;
import com.kaer.sdk.OnClientCallback;
import com.kaer.sdk.bt.BtReadClient;
import com.kaer.sdk.bt.OnBluetoothListener;
import com.kaer.sdk.utils.CardCode;

import java.io.ByteArrayOutputStream;
import java.io.IOException;

public class Kaer extends CordovaPlugin implements OnClientCallback, OnBluetoothListener {
    public static CallbackContext currentCallbackContext;
    public static String TAG = "cordova-plugin-kaer";

    private final String ip = "115.28.2.173";
    private final int port = 7443;
    private final String username = "admin";
    private final String password = "www.kaer.cn";
    private BtReadClient mBtReadClient;


    @Override
    public void initialize(CordovaInterface cordova, CordovaWebView webView) {
        super.initialize(cordova, webView);
        mBtReadClient = BtReadClient.getInstance();
        mBtReadClient.setClientCallback(this);
        int connected = mBtReadClient.init(cordova.getActivity(), ip, port, username, getMD5(password.getBytes()));
        Log.i(TAG, "connected: " + connected);
    }

    @Override
    public boolean execute(String action, CordovaArgs args, CallbackContext callbackContext) throws JSONException {
        if (action.equals("readIdCard")) {
            return this.readIdCard(args.getString(0), callbackContext);
        }
        callbackContext.error("Unkown action");
        return false;
    }

    protected boolean readIdCard(String uuid, CallbackContext callbackContext) {
        mBtReadClient.setBluetoothListener(this);
        if(mBtReadClient.connectBt(uuid)) {
            Log.i(TAG, "连接蓝牙成功");
            currentCallbackContext = callbackContext;
            new ReadAsync().execute();
        } else {
            callbackContext.error("连接蓝牙失败");
        }

        return true;
    }

    @Override
    public void preExcute(long l) {

    }

    @Override
    public void updateProgress(int i) {
        Log.i(TAG, "updateProgress: " + i);
        JSONObject res = new JSONObject();
        try {
            res.put("status", "progress");
            res.put("progress", i / 100);
        } catch (JSONException e) {
            currentCallbackContext.error(e.getLocalizedMessage());
        }
        PluginResult result = new PluginResult(PluginResult.Status.OK, res);
        result.setKeepCallback(true);
        currentCallbackContext.sendPluginResult(result);
    }

    @Override
    public void onConnectChange(int i) {
        Log.i(TAG, "onConnectChange: " + i);
    }

    @Override
    public void connectResult(boolean b) {
        Log.i(TAG, "connectResult: " + b);
    }

    @Override
    public void connectionLost() {

    }

    class ReadAsync extends AsyncTask<Intent, Integer, IDCardItem> {
        @Override
        protected void onPreExecute() {
            // TODO Auto-generated method stub
            super.onPreExecute();
            Log.i(TAG, "onPreExecute");
        }

        @Override
        protected IDCardItem doInBackground(Intent... params) {
            // TODO Auto-generated method stub
            Log.i(TAG, "开始读卡");
            IDCardItem item = mBtReadClient.readCert();
            return item;
        }

        @Override
        protected void onPostExecute(IDCardItem idCardItem) {
            // TODO Auto-generated method stub
            super.onPostExecute(idCardItem);
            Log.i(TAG, "onPostExecute: " + idCardItem.toString());
            if(idCardItem.retCode != 1) {
                currentCallbackContext.error(CardCode.errorCodeDescription(idCardItem.retCode));
                return;
            }
            String image = "data:image/jpeg;base64," + bitmapToBase64(idCardItem.picBitmap);
            JSONObject res = new JSONObject();
            JSONObject data = new JSONObject();
            try {
                data.put("name", idCardItem.partyName);
                data.put("image", image);
                data.put("gender", idCardItem.gender);
                data.put("ethnic", idCardItem.nation);
                data.put("dateOfBirth", idCardItem.bornDay);
                data.put("address", idCardItem.certAddress);
                data.put("id", idCardItem.certNumber);
                data.put("org", idCardItem.certOrg);
                data.put("effDate", idCardItem.effDate);
                data.put("expDate", idCardItem.expDate);
                res.put("status", "completed");
                res.put("data", data);
            } catch (JSONException e) {
                currentCallbackContext.error(e.getLocalizedMessage());
                return;
            }
            PluginResult result = new PluginResult(PluginResult.Status.OK, res);
            currentCallbackContext.sendPluginResult(result);
        }
    }

    private String getMD5(byte[] source) {
        String s = null;
        char hexDigits[] = { // 用来将字节转换成 16 进制表示的字符
                '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd',
                'e', 'f' };
        try {
            java.security.MessageDigest md = java.security.MessageDigest
                    .getInstance("MD5");
            md.update(source);
            byte tmp[] = md.digest(); // MD5 的计算结果是一个 128 位的长整数，
            // 用字节表示就是 16 个字节
            char str[] = new char[16 * 2]; // 每个字节用 16 进制表示的话，使用两个字符，
            // 所以表示成 16 进制需要 32 个字符
            int k = 0; // 表示转换结果中对应的字符位置
            for (int i = 0; i < 16; i++) { // 从第一个字节开始，对 MD5 的每一个字节
                // 转换成 16 进制字符的转换
                byte byte0 = tmp[i]; // 取第 i 个字节
                str[k++] = hexDigits[byte0 >>> 4 & 0xf]; // 取字节中高 4 位的数字转换,
                // >>> 为逻辑右移，将符号位一起右移
                str[k++] = hexDigits[byte0 & 0xf]; // 取字节中低 4 位的数字转换
            }
            s = new String(str); // 换后的结果转换为字符串

        } catch (Exception e) {
            e.printStackTrace();
        }
        return s;
    }

    private String bitmapToBase64(Bitmap bitmap) {
        String result = null;
        ByteArrayOutputStream baos = null;
        try {
            if (bitmap != null) {
                baos = new ByteArrayOutputStream();
                bitmap.compress(Bitmap.CompressFormat.JPEG, 100, baos);

                baos.flush();
                baos.close();

                byte[] bitmapBytes = baos.toByteArray();
                result = Base64.encodeToString(bitmapBytes, Base64.DEFAULT);
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                if (baos != null) {
                    baos.flush();
                    baos.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return result;
    }
}