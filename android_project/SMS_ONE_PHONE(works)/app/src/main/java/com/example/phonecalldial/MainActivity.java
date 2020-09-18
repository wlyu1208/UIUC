package com.example.phonecalldial;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.telephony.SmsManager;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    EditText txt_pNumber, txt_message;
    String phoneNumber;
    String Message;
    Button button_active;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        // find values from layout based on the ids
        txt_message = (EditText)findViewById(R.id.txt_message);
        txt_pNumber = (EditText)findViewById(R.id.txt_phone_number);
        button_active = (Button)findViewById(R.id.send_button);

        //if button is pressed
        button_active.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                int permissionCheck = ContextCompat.checkSelfPermission(MainActivity.this, Manifest.permission.SEND_SMS);

                // permission check and send if msg is not empty or not send if no permission
                if(permissionCheck== PackageManager.PERMISSION_GRANTED){
                    phoneNumber = txt_pNumber.getText().toString().trim();
                    Message = txt_message.getText().toString().trim();

                    if(!txt_pNumber.getText().toString().equals("") || !txt_message.getText().toString().equals("")){
                        SmsManager smsManager = SmsManager.getDefault();
                        smsManager.sendTextMessage(phoneNumber, null, Message, null, null);

                        Toast.makeText(MainActivity.this, "Message Sent", Toast.LENGTH_SHORT).show();
                    }
                    else{
                        Toast.makeText(MainActivity.this, "Please Enter Number or Message", Toast.LENGTH_SHORT).show();
                    }
                }
                else{
                    ActivityCompat.requestPermissions(MainActivity.this, new String[]{Manifest.permission.SEND_SMS}, 0 );
                }
            }
        });

    }

    // request permission if it is permission was neither allow or deny
    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);

        switch (requestCode){
            case 0:
                if(grantResults.length>=0 && grantResults[0]==PackageManager.PERMISSION_GRANTED){
                    Toast.makeText(this, "Permission Granted", Toast.LENGTH_SHORT).show();
                    phoneNumber = txt_pNumber.getText().toString().trim();
                    Message = txt_message.getText().toString().trim();

                    if(!txt_pNumber.getText().toString().equals("") || !txt_message.getText().toString().equals("")){
                        SmsManager smsManager = SmsManager.getDefault();
                        smsManager.sendTextMessage(phoneNumber, null, Message, null, null);

                        Toast.makeText(this, "Message Sent", Toast.LENGTH_SHORT).show();
                    }
                    else{
                        Toast.makeText(this, "Please Enter Number or Message", Toast.LENGTH_SHORT).show();
                    }
                }
                else{
                    Toast.makeText(this, "You do not have Required Permission", Toast.LENGTH_SHORT).show();
                }
                break;
        }
    }
}