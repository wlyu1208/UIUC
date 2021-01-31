package com.example.content_provider;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

import androidx.annotation.Nullable;

public class dbHelper extends SQLiteOpenHelper {
    private static final String DATABASE_NAME = "inbox.db";

    public static final String Table_Name = "Messages";
    public static final String col_1 = "_id";
    public static final String col_2 = "phone_num";
    public static final String col_3 = "test_msg";

    public dbHelper(@Nullable Context context, @Nullable String name, @Nullable SQLiteDatabase.CursorFactory factory, int version) {
        super(context, DATABASE_NAME, null, 1);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        db.execSQL("CREATE TABLE " + Table_Name + " (" + col_1 + " INTEGER PRIMARY KEY AUTOINCREMENT, " + col_1 + " TEXT, " + col_2 + " TEXT, " + col_3 + " TEXT)");
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        db.execSQL("DROP TABLE IF EXISTS " + Table_Name);
        onCreate(db);
    }
}
