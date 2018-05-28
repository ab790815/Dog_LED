package com.cheesemoon_.dog_led_bluetooth;

import android.content.SharedPreferences;

/**
 * Created by a on 2016/7/11.
 * 儲存 及 讀取 資料模組
 * <p>
 * 以XML形式儲存
 * 以 Tag 與 資料內容 做配對
 * 只要知道 Tag 名稱就可以存取資料
 */
public class Save_and_Read_Database {
    public SharedPreferences settings;

    public void save_BT_ADD(String SERVER_IP) {
        settings.edit().putString("BT_ADD", SERVER_IP).apply();
    }

    public String read_BT_ADD() {
        return settings.getString("BT_ADD", "98:D3:32:10:B8:A0");
    }

}





















































































































