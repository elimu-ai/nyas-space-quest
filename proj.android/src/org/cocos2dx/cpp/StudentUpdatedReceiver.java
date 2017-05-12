package org.cocos2dx.cpp;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.util.Log;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.Set;

public class StudentUpdatedReceiver extends BroadcastReceiver {

    public static final String PREF_STUDENT_LETTERS = "pref_student_letters";
    public static final String PREF_STUDENT_NUMBERS = "pref_student_numbers";

    @Override
    public void onReceive(Context context, Intent intent) {
        Log.i(getClass().getName(), "onReceive");

        ArrayList<String> availableNumbers = intent.getStringArrayListExtra("availableNumbers");
        Log.i(getClass().getName(), "availableNumbers: " + availableNumbers);
    }
}