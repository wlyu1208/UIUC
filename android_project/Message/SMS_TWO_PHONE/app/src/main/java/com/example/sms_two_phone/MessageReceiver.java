package com.example.sms_two_phone;

import android.annotation.TargetApi;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.telephony.SmsMessage;
import android.util.Log;
import android.widget.Toast;

import static android.content.ContentValues.TAG;

public class MessageReceiver extends BroadcastReceiver {
    private static final String TAG = "MessageReceiver";
    private static final String SMS_RECEIVED = "android.provider.Telephony.SMS_RECEIVED";
    String msg, phoneNo = "";
    @TargetApi(Build.VERSION_CODES.M)
    @Override
    public void onReceive(Context context, Intent intent){
        //get message pass in

        Log.i(TAG, "Intent Received" + intent.getAction());
        if(intent.getAction() == SMS_RECEIVED){
            Bundle databundle = intent.getExtras();
            String format = databundle.getString("format");
            if(databundle != null){
                boolean isVersionM = (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M);
                Object[] pdus = (Object[]) databundle.get("pdus");
                final SmsMessage[] message = new SmsMessage[pdus.length];
                for(int i=0; i<pdus.length; i++){
                    if(isVersionM){
                        message[i] = SmsMessage.createFromPdu((byte[]) (pdus[i]), format);
                    }else{
                        message[i] = SmsMessage.createFromPdu((byte[]) pdus[i]);
                    }
                    msg = message[i].getMessageBody();
                    phoneNo = message[i].getOriginatingAddress();
                }
                Toast.makeText(context, "Message: " + msg + "\nNumber: " + phoneNo, Toast.LENGTH_LONG).show();
            }
        }
//        Bundle bundle = intent.getExtras();
//        SmsMessage[] messages;
//        String str = "";
//        String format = bundle.getString("format");
//
//        if(bundle != null){
//            boolean isVersionM = (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M);
//            Object[] pdus = (Object[]) bundle.get("pdus");
//            messages = new SmsMessage[pdus.length];
//            for(int i=0; i<messages.length; i++) {
//                if(isVersionM){
//                    messages[i] = SmsMessage.createFromPdu((byte[]) (pdus[i]), format);
//                }
//                else{
//                    messages[i] = SmsMessage.createFromPdu((byte[]) pdus[i]);
//                }
//
//                str += messages[i].getOriginatingAddress();
//                str += ": ";
//                str += messages[i].getMessageBody().toString();
//                str+= "\n";
//            }
//            Log.d(TAG, "onReceive: "+str);
//            Toast.makeText(context, str, Toast.LENGTH_SHORT).show();
//
//            Intent broadcastIntent = new Intent();
//            broadcastIntent.setAction("SMS_RECEIVED_ACTION");
//            broadcastIntent.putExtra("sms", str);
//            context.sendBroadcast(broadcastIntent);
//        }
    }
}
