package com.example.message_with_database;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.nfc.Tag;
import android.util.Log;

import androidx.annotation.Nullable;
import androidx.core.content.ContextCompat;

public class Database_helper extends SQLiteOpenHelper {
    public static final String DATABASE_NAME = "Message.db";
    public static final String TABLE_NAME = "Message_List";
    public static final String COL_1 = "Phone_Number";
    public static final String COL_2 = "Text_List";

    public Database_helper(Context context) {
        super(context, DATABASE_NAME, null, 1);
        SQLiteDatabase db = this.getWritableDatabase();
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        // create table Message_List Phone_Number Integer Primary Key, Text_List text
        db.execSQL("CREATE TABLE " + TABLE_NAME + " (Phone_Number INTEGER, " + COL_2 + " TEXT)");
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        db.execSQL("Drop table if exists " + TABLE_NAME);
        onCreate(db);
    }

    public boolean addData(String phone_no, String message){
        SQLiteDatabase db = this.getWritableDatabase();
        ContentValues contentValues = new ContentValues();
        contentValues.put(COL_1, phone_no);
        contentValues.put(COL_2, message);

        long result = db.insert(TABLE_NAME, null, contentValues);
        if(result == -1){
            return false;
        }
        else{
            return true;
        }
    }

    public Cursor get_all_data(){
        SQLiteDatabase db = this.getWritableDatabase();
        Cursor rest = db.rawQuery("select * from " + TABLE_NAME, null);
        return rest;
    }
}
