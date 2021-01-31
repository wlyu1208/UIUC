package com.example.message_with_database;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.database.Cursor;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {
    Database_helper myDb;
    EditText phone, message;
    Button addData;
    Button viewData;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        myDb = new Database_helper(this);
        phone = (EditText)findViewById(R.id.phone_number);
        message = (EditText)findViewById(R.id.msg);
        addData = (Button)findViewById(R.id.send_btn);
        viewData = (Button)findViewById(R.id.view_btn);

        Add_To_Table();
        View_All_Table();
    }

    public void Add_To_Table(){
        addData.setOnClickListener(
                new View.OnClickListener(){
                    @Override
                    public void onClick(View v) {
                        boolean insert = myDb.addData(phone.getText().toString(), message.getText().toString());
                        if(insert = true){
                            Toast.makeText(MainActivity.this, "Data inserted", Toast.LENGTH_LONG).show();
                        }
                        else{
                            Toast.makeText(MainActivity.this, "Not inserted", Toast.LENGTH_LONG).show();
                        }
                    }
                }
        );
    }

    public void View_All_Table(){
        viewData.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        Cursor rest = myDb.get_all_data();
                        if(rest.getCount()==0){
                            showMessage("Error", "Nothing found");
                            return;
                        }
                        else{
                            StringBuffer buffer = new StringBuffer();
                            while(rest.moveToNext()){
                                buffer.append("Phone Number: " + rest.getString(0)+ "\n");
                                buffer.append("Text: " + rest.getString(1)+ "\n");
                            }
                            showMessage("Data", buffer.toString());
                        }
                    }
                }
        );
    }

    public void showMessage(String title, String message){
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setCancelable(true);
        builder.setTitle(title);
        builder.setMessage(message);
        builder.show();
    }
}