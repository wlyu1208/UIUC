package com.example.text_database_content;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.database.Cursor;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RelativeLayout;
import android.widget.Toast;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;

public class MainActivity extends AppCompatActivity {
    database_helper mydb;
    EditText edit_phone_number;
    EditText edit_text;
    Button add_button;
    Button view_button;
    Date current_time;
    Button recycler_button;
    ArrayList<String> id_list;
    ArrayList<String> number_list;
    ArrayList<String> message_list;
    ArrayList<String> time_list;
    Recycler_Adapter recyclerAdapter;
    RecyclerView recyclerView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mydb = new database_helper(this);

        recyclerView = findViewById(R.id.recyclerView);
        edit_phone_number = (EditText) findViewById(R.id.number_data);
        edit_text = (EditText) findViewById(R.id.text_data);
        add_button = (Button) findViewById(R.id.add_message);
        view_button = (Button) findViewById(R.id.view_messages);
        current_time = Calendar.getInstance().getTime();
        recycler_button = (Button) findViewById(R.id.view_recycler);

        id_list = new ArrayList<>();
        number_list = new ArrayList<>();
        message_list = new ArrayList<>();
        time_list = new ArrayList<>();

        add_data();
        view_data();
//        view_in_recycler();
//
//        recyclerView.setLayoutManager(new LinearLayoutManager(MainActivity.this));
//        recyclerAdapter = new Recycler_Adapter(MainActivity.this, id_list, number_list, message_list, time_list);
//        recyclerView.setAdapter(recyclerAdapter);
    }

    public void add_data(){
        add_button.setOnClickListener(
                new View.OnClickListener(){
                    @Override
                    public void onClick(View v) {
                        if(TextUtils.isEmpty(edit_phone_number.getText()) || TextUtils.isEmpty((edit_text.getText()))){
                            Toast.makeText(MainActivity.this, "Number or Message is Empty", Toast.LENGTH_LONG).show();
                            return;
                        }
                        boolean success_add = mydb.insert_data(edit_phone_number.getText().toString(),
                                edit_text.getText().toString(), current_time.toString());
                        if(success_add){
                            Toast.makeText(MainActivity.this, "Successful", Toast.LENGTH_LONG).show();
                        }
                        else{
                            Toast.makeText(MainActivity.this, "Failed", Toast.LENGTH_LONG).show();
                        }
                    }
                }
        );
    }

    public void view_data(){
        view_button.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        Cursor result = mydb.viewdata();
                        if(result.getCount() == 0){
                            show_messages("Error", "No Message");
                            return;
                        }
                        else{
                            StringBuffer buffer = new StringBuffer();
                            while(result.moveToNext()){
                                buffer.append("ID: " + result.getString(0)+"\n");
                                buffer.append("Number: " + result.getString(1)+"\n");
                                buffer.append("Message: " + result.getString(2)+"\n");
                                buffer.append("Time: " + result.getString(3)+"\n");
                                buffer.append("\n");
                            }
                            show_messages("Messages", buffer.toString());
                        }
                    }
                }
        );
    }

    public void show_messages(String title, String message){
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setCancelable(true);
        builder.setTitle(title);
        builder.setMessage(message);
        builder.show();
    }

    public void view_in_recycler(){
        recycler_button.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        recyclerView.setLayoutManager(new LinearLayoutManager(MainActivity.this));
                        recyclerAdapter = new Recycler_Adapter(MainActivity.this, id_list, number_list, message_list, time_list);
                        recyclerView.setAdapter(recyclerAdapter);

                        Cursor result = mydb.viewdata();
                        if(result.getCount() == 0){
                            show_messages("Error", "No Message");
                            return;
                        }
                        else{
                            while (result.moveToNext()){
                                id_list.add(result.getString(0));
                                number_list.add(result.getString(1));
                                message_list.add(result.getString(2));
                                time_list.add(result.getString(3));
                            }
                        }
                    }
                }
        );
    }

}