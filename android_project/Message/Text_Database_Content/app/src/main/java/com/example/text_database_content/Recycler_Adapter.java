package com.example.text_database_content;

import android.content.Context;
import android.text.Layout;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import java.util.ArrayList;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

public class Recycler_Adapter extends RecyclerView.Adapter<Recycler_Adapter.MyViewHolder>{

    Context context;
    ArrayList id_array, phone_array, message_array, time_array;
    //provide reference to views for each data item
    Recycler_Adapter(Context context, ArrayList id_array, ArrayList phone_array, ArrayList message_array, ArrayList time_array){
        this.context = context;
        this.id_array = id_array;
        this.phone_array = phone_array;
        this.message_array = message_array;
        this.time_array = time_array;
    }

    public class MyViewHolder extends RecyclerView.ViewHolder{
        TextView phone_id_text, phone_num_text, phone_msg_text, phone_time_text;
        public MyViewHolder(@NonNull View itemView) {
            super(itemView);
            phone_id_text = itemView.findViewById(R.id.text_id);
            phone_num_text = itemView.findViewById(R.id.text_number);
            phone_msg_text = itemView.findViewById(R.id.text_message);
            phone_time_text = itemView.findViewById(R.id.text_time);
        }
    }
    //connect layout with views, create new views
    @NonNull
    @Override
    public MyViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        LayoutInflater inflater = LayoutInflater.from(context);
        View view = inflater.inflate(R.layout.recycler_row, parent, false);
        return new MyViewHolder(view);
    }

    //replace contents of view
    @Override
    public void onBindViewHolder(@NonNull MyViewHolder holder, int position) {
        holder.phone_id_text.setText(String.valueOf(id_array.get(position)));
        holder.phone_num_text.setText(String.valueOf(phone_array.get(position)));
        holder.phone_msg_text.setText(String.valueOf(message_array.get(position)));
        holder.phone_time_text.setText(String.valueOf(time_array.get(position)));
    }
    //return size of dataset
    @Override
    public int getItemCount() {
        return id_array.size();
    }

}
