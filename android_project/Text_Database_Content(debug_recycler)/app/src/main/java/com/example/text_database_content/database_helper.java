package com.example.text_database_content;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

import androidx.annotation.Nullable;

public class database_helper extends SQLiteOpenHelper {
    public static final String Database_name = "message_list.db";
    public static final String Table_name = "Message_Table";
    public static final String col_1 = "ID";
    public static final String col_2 = "phone_number";
    public static final String col_3 = "text_content";
    public static final String col_4 = "time";

    public database_helper(@Nullable Context context) {
        super(context, Database_name, null, 1);
        SQLiteDatabase db = this.getWritableDatabase();
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        db.execSQL("CREATE TABLE " + Table_name + " (ID INTEGER PRIMARY KEY AUTOINCREMENT, phone_number INTEGER, text_content TEXT, time TEXT) ");
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        db.execSQL("DROP TABLE IF EXISTS " + Table_name);
        onCreate(db);
    }

    public boolean insert_data(String number, String content, String time){
        SQLiteDatabase db = this.getWritableDatabase();
        ContentValues contentValues = new ContentValues();
        contentValues.put(col_2, number);
        contentValues.put(col_3, content);
        contentValues.put(col_4, time);
        long result = db.insert(Table_name, null, contentValues);
        if(result == -1){
            return false;
        }
        else{
            return true;
        }
    }

    public Cursor viewdata(){
        SQLiteDatabase db = this.getReadableDatabase();
        Cursor result = db.rawQuery("SELECT * FROM " + Table_name, null);
        return result;
    }
}
