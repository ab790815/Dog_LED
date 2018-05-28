package com.cheesemoon_.dog_led_bluetooth;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.Toast;

import java.time.Instant;
import java.util.ArrayList;
import java.util.Set;

public class MainActivity extends AppCompatActivity {
    private Button list;
    private BluetoothAdapter BA;
    private Set<BluetoothDevice> pairedDevices;
    private ListView lv;

    ArrayList<String> BT_list = new ArrayList<>();
    ArrayList<String> BT_Address_list = new ArrayList<>();
    Save_and_Read_Database SARD;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_new);

        SARD = new Save_and_Read_Database();
        SARD.settings = getSharedPreferences("DATA", 0);

        BA = BluetoothAdapter.getDefaultAdapter();
        list = (Button) findViewById(R.id.button4);

        lv = (ListView) findViewById(R.id.listView1);
        lv.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                Log.i("onItemClick", "" + BT_Address_list.get(i));
                String device_code = BT_Address_list.get(i);

                SARD.save_BT_ADD(device_code);

                Intent intent = new Intent(MainActivity.this, Ctrl_Page.class);
                startActivity(intent);
            }
        });


    }

    public void list(View view) {
        BT_list = new ArrayList<>();
        BT_Address_list = new ArrayList<>();


        pairedDevices = BA.getBondedDevices();

        for (BluetoothDevice bt : pairedDevices) {
            BT_list.add(bt.getName());
            BT_Address_list.add(bt.getAddress());
        }

        Toast.makeText(getApplicationContext(), "Showing Paired Devices",
                Toast.LENGTH_SHORT).show();
        final ArrayAdapter<String> adapter = new ArrayAdapter<>
                (this, android.R.layout.simple_list_item_1, BT_list);
        lv.setAdapter(adapter);
    }

}
