package com.WhenInDoubtC4.ApologyCounter;

import android.annotation.TargetApi;
import android.app.Activity;
import android.app.AlertDialog;
import android.appwidget.AppWidgetManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Build;
import android.text.Html;
import android.text.InputType;
import android.view.View;
import android.view.WindowManager;
import android.widget.EditText;
import android.widget.RemoteViews;

import java.io.File;

public class AndroidUtils
{
    private static native void createNewCounter(String name);
    private static native void deleteCounter();
    public static native Activity getAndroidActivity();
    public static native String getWidgetName(int index);
    public static native String getWidgetDisplayName(int index);
    public static native int getCountForName(String name);
    public static native void incrementCounter(String name);
    public static native void updateWidgetChart();

    @TargetApi(Build.VERSION_CODES.CUPCAKE)
    public void displayNewCounterMessageBox(Activity activity)
    {
        activity.runOnUiThread(() -> {
            AlertDialog.Builder builder = new AlertDialog.Builder(activity);

            builder.setTitle("New counter");
            builder.setMessage("Type the name of the person who apologizes a lot");

            EditText input = new EditText(activity);
            input.setHint("Name");
            input.setInputType(InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_PERSON_NAME);
            builder.setView(input);

            builder.setPositiveButton("OK", new DialogInterface.OnClickListener()
            {
                public void onClick(DialogInterface dialog, int id)
                {
                    createNewCounter(input.getText().toString());
                }
            });

            builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener()
            {
                public void onClick(DialogInterface dialog, int id) {}
            });

            AlertDialog dialog = builder.create();
            dialog.show();

            //Show keyboard automatically when input gets focused
            input.setOnFocusChangeListener(new View.OnFocusChangeListener() {
                @Override
                public void onFocusChange(View v, boolean hasFocus) {
                    if (hasFocus) {
                        dialog.getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_VISIBLE);
                    }
                }
            });

            input.requestFocus();
        });
    }

    public void displayDeleteCounterMessageBox(Activity activity, String displayName)
    {
        activity.runOnUiThread(() -> {
            AlertDialog.Builder builder = new AlertDialog.Builder(activity);

            builder.setTitle("Delete counter");
            builder.setMessage(Html.fromHtml("Are your sure you want to delete the counter for <b>" + displayName + "</b>? This will delete all the stats and data!"));

            builder.setPositiveButton("Delete", new DialogInterface.OnClickListener()
            {
                public void onClick(DialogInterface dialog, int id)
                {
                    deleteCounter();
                }
            });

            builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener()
            {
                public void onClick(DialogInterface dialog, int id)
                {
                }
            });

            AlertDialog dialog = builder.create();
            dialog.show();
        });
    }

    public void requestUpdateWidget()
    {
        Intent intent = new Intent(getAndroidActivity(), AndroidWidget.class);
        intent.setAction(AppWidgetManager.ACTION_APPWIDGET_UPDATE);
        int[] ids = AppWidgetManager.getInstance(getAndroidActivity().getApplication())
                .getAppWidgetIds(new ComponentName(getAndroidActivity().getApplication(), AndroidWidget.class));
        intent.putExtra(AppWidgetManager.EXTRA_APPWIDGET_IDS, ids);
        getAndroidActivity().sendBroadcast(intent);
    }
}
