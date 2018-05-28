package com.cheesemoon_.dog_led_bluetooth;

import android.content.Context;
import android.graphics.Color;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;

import org.w3c.dom.Text;

public class Ctrl_Page extends AppCompatActivity {
    private BT bt;
    int Color_R = 0;
    int Color_G = 0;
    int Color_B = 0;
    int Time = 0;
    int R_L = 0;
    int Mis = 0;
    Save_and_Read_Database SARD;

    Context context;
    private Button btnColorPicker;
    private TextView tvText;
    private ImageView img;

    SeekBar seekBar_R;
    SeekBar seekBar_G;
    SeekBar seekBar_B;
    SeekBar seekBar_T;
    TextView textView_R;
    TextView textView_G;
    TextView textView_B;
    TextView textView_T;
//    Button Submit;

    byte[] data;
    Switch 左或右;
    Button 關閉;
    Button RGB;
    Button RGB閃爍;
    Button RGB環繞;
    Button 彩虹;
    Button 彩虹環繞;
    TextView textView_ASD;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        context = this;
        super.onCreate(savedInstanceState);
        setContentView(R.layout.ctrl_page);
        SARD = new Save_and_Read_Database();
        SARD.settings = getSharedPreferences("DATA", 0);
        BT.address = SARD.read_BT_ADD();
        bt = new BT();
        textView_ASD = (TextView) findViewById(R.id.TestView_ASD);
        textView_ASD.setBackgroundColor(Color.argb(255, Color_R, Color_G, Color_B));
        init();
//        Submit = (Button) findViewById(R.id.Submit);
//        Submit.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View view) {
//                update_information();
//            }
//        });
    }

    void init() {
        data = new byte[]{0x24, 0x44, 0x3E, 0x0, 0x0, 0x0};
        seekBar_R = (SeekBar) findViewById(R.id.SeekBar_R);
        seekBar_G = (SeekBar) findViewById(R.id.SeekBar_G);
        seekBar_B = (SeekBar) findViewById(R.id.SeekBar_B);
        seekBar_T = (SeekBar) findViewById(R.id.SeekBar_T);
        textView_R = (TextView) findViewById(R.id.TextView_R);
        textView_G = (TextView) findViewById(R.id.TextView_G);
        textView_B = (TextView) findViewById(R.id.TextView_B);
        textView_T = (TextView) findViewById(R.id.TextView_T);

        seekBar_R.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                Color_R = i;
                textView_R.setText("R : " + i);
                textView_ASD.setBackgroundColor(Color.argb(255, Color_R, Color_G, Color_B));

            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });
        seekBar_G.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                Color_G = i;
                textView_G.setText("G : " + i);
                textView_ASD.setBackgroundColor(Color.argb(255, Color_R, Color_G, Color_B));
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });
        seekBar_B.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                Color_B = i;
                textView_B.setText("B : " + i);
                textView_ASD.setBackgroundColor(Color.argb(255, Color_R, Color_G, Color_B));
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });
        seekBar_T.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                Time = i;
                textView_T.setText("T : " + i + " s");
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        左或右 = (Switch) findViewById(R.id.左或右);
        關閉 = (Button) findViewById(R.id.關閉);
        RGB = (Button) findViewById(R.id.RGB);
        RGB閃爍 = (Button) findViewById(R.id.RGB閃爍);
        RGB環繞 = (Button) findViewById(R.id.RGB環繞);
        彩虹 = (Button) findViewById(R.id.彩虹);
        彩虹環繞 = (Button) findViewById(R.id.彩虹環繞);

        關閉.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Mis = 0;
                data = new byte[]{0x24, 0x44, 0x3E, 0x0, 0x0, 0x0};
                update_information();
            }
        });
        RGB.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Mis = 1;
                data = new byte[9];
                data[0] = 0x24;
                data[1] = 0x44;
                data[2] = 0x3E;
                data[3] = 0x01;
                data[4] = 0x3;
                data[5] = (byte) Color_R;
                data[6] = (byte) Color_G;
                data[7] = (byte) Color_B;
                data[8] = CheckSum(data);
                update_information();
            }
        });
        RGB閃爍.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Mis = 2;
                data = new byte[10];
                data[0] = 0x24;
                data[1] = 0x44;
                data[2] = 0x3E;
                data[3] = 0x02;
                data[4] = 0x4;
                data[5] = (byte) Color_R;
                data[6] = (byte) Color_G;
                data[7] = (byte) Color_B;
                data[8] = (byte) Time;
                data[9] = CheckSum(data);
                update_information();
            }
        });
        RGB環繞.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Mis = 3;
                data = new byte[11];
                data[0] = 0x24;
                data[1] = 0x44;
                data[2] = 0x3E;
                data[3] = 0x03;
                data[4] = 0x5;
                data[5] = (byte) Color_R;
                data[6] = (byte) Color_G;
                data[7] = (byte) Color_B;
                data[8] = (byte) Time;
                R_L = (左或右.isChecked()) ? 1 : 0;
                data[9] = (byte) R_L;
                data[10] = CheckSum(data);
                update_information();
            }
        });
        彩虹.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Mis = 4;
                data = new byte[6];
                data[0] = 0x24;
                data[1] = 0x44;
                data[2] = 0x3E;
                data[3] = 0x04;
                data[4] = 0x1;
                data[5] = CheckSum(data);
                update_information();
            }
        });
        彩虹環繞.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Mis = 5;
                data = new byte[7];
                data[0] = 0x24;
                data[1] = 0x44;
                data[2] = 0x3E;
                data[3] = 0x05;
                data[4] = 0x1;
                data[5] = (byte) Time;
                data[6] = CheckSum(data);
                update_information();
            }
        });
        /*
            int Color_R = 0;
    int Color_G = 0;
    int Color_B = 0;
    int Time = 0;
    int R_L = 0;
        * */

    }

    byte CheckSum(byte[] data) {
        byte CS = 0x00;
        for (int i = 4; i < data.length - 1; i++) {
            CS = (byte) (CS ^ (data[i]));
        }
        return CS;
    }

    void update_information() {
        String str = "";
        for (int i = 0; i < data.length; i++) {
            str += "data[" + i + "] = " + (data[i] & 0xFF) + " ";
        }
        Log.i("str", "" + str);
        bt.mConnectedThread.write(data);
    }

}
