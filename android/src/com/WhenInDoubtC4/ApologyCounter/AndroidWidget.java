package com.WhenInDoubtC4.ApologyCounter;

import android.annotation.TargetApi;
import android.app.PendingIntent;
import android.appwidget.AppWidgetManager;
import android.appwidget.AppWidgetProvider;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.RemoteViews;
import android.widget.Toast;

import java.io.File;

@TargetApi(Build.VERSION_CODES.CUPCAKE)
public class AndroidWidget extends AppWidgetProvider
{
    private static final String ACTION_INCREMENT_1 = "ACTION_INCREMENT_1";
    private static final String ACTION_INCREMENT_2 = "ACTION_INCREMENT_2";

    private static final int RESIZE_WIDTH = 500;
    private static final int RESIZE_HEIGHT = 200;

    protected static PendingIntent getPendingSelfIntent(Context context, String action)
    {
        Intent intent = new Intent(context, AndroidWidget.class);
        intent.setAction(action);
        return PendingIntent.getBroadcast(context, 0, intent, PendingIntent.FLAG_UPDATE_CURRENT);
    }

    @Override
    public void onAppWidgetOptionsChanged(Context context, AppWidgetManager appWidgetManager, int appWidgetId, Bundle newOptions)
    {
        RemoteViews views = new RemoteViews(context.getPackageName(), R.layout.android_widget);

        int maxWidth = newOptions.getInt(AppWidgetManager.OPTION_APPWIDGET_MAX_WIDTH);
        int maxHeight = newOptions.getInt(AppWidgetManager.OPTION_APPWIDGET_MAX_HEIGHT);

        if (maxWidth < RESIZE_WIDTH && maxHeight < RESIZE_HEIGHT)
        {
            views.setViewVisibility(R.id.android_widget_layout2, View.GONE);
            views.setViewVisibility(R.id.android_widget_bottom_row, View.GONE);
            views.setViewVisibility(R.id.android_widget_chart0, View.GONE);
            views.setViewVisibility(R.id.android_widget_chart1, View.GONE);
        }
        else if (maxWidth >= RESIZE_WIDTH && maxHeight < RESIZE_HEIGHT)
        {
            views.setViewVisibility(R.id.android_widget_layout2, View.VISIBLE);
            views.setViewVisibility(R.id.android_widget_bottom_row, View.GONE);
            views.setViewVisibility(R.id.android_widget_chart0, View.GONE);
            views.setViewVisibility(R.id.android_widget_chart1, View.GONE);
        }
        else if (maxWidth < RESIZE_WIDTH && maxHeight >= RESIZE_HEIGHT)
        {
            views.setViewVisibility(R.id.android_widget_layout2, View.GONE);
            views.setViewVisibility(R.id.android_widget_bottom_row, View.VISIBLE);
            views.setViewVisibility(R.id.android_widget_chart0, View.VISIBLE);
            views.setViewVisibility(R.id.android_widget_chart1, View.GONE);
        }
        else
        {
            views.setViewVisibility(R.id.android_widget_layout2, View.VISIBLE);
            views.setViewVisibility(R.id.android_widget_bottom_row, View.VISIBLE);
            views.setViewVisibility(R.id.android_widget_chart0, View.VISIBLE);
            views.setViewVisibility(R.id.android_widget_chart1, View.VISIBLE);
        }

        updateWidget(views, context, appWidgetManager, appWidgetId);
    }

    @Override
    public void onUpdate(Context context, AppWidgetManager appWidgetManager, int[] appWidgetIds)
    {
        RemoteViews views = new RemoteViews(context.getPackageName(), R.layout.android_widget);
        for (int appWidgetID : appWidgetIds) updateWidget(views, context, appWidgetManager, appWidgetID);
    }

    private static void updateWidget(RemoteViews views, Context context, AppWidgetManager appWidgetManager, int appWidgetID)
    {
        views.setTextViewText(R.id.android_widget_name1, AndroidUtils.getWidgetDisplayName(0));
        views.setTextViewText(R.id.android_widget_count1, String.valueOf(AndroidUtils.getCountForName(AndroidUtils.getWidgetName(0))));
        views.setOnClickPendingIntent(R.id.android_widget_increment1, getPendingSelfIntent(context, ACTION_INCREMENT_1));
        views.setTextViewText(R.id.android_widget_name2, AndroidUtils.getWidgetDisplayName(1));
        views.setTextViewText(R.id.android_widget_count2, String.valueOf(AndroidUtils.getCountForName(AndroidUtils.getWidgetName(1))));
        views.setOnClickPendingIntent(R.id.android_widget_increment2, getPendingSelfIntent(context, ACTION_INCREMENT_2));

        AndroidUtils.updateWidgetChart();
        File chart0File = new File(context.getFilesDir().toString(), "chart0.png");
        Bitmap chart0Bitmap = BitmapFactory.decodeFile(chart0File.getAbsolutePath());
        views.setImageViewBitmap(R.id.android_widget_chart0, chart0Bitmap);
        File chart1File = new File(context.getFilesDir().toString(), "chart1.png");
        Bitmap chart1Bitmap = BitmapFactory.decodeFile(chart1File.getAbsolutePath());
        views.setImageViewBitmap(R.id.android_widget_chart1, chart1Bitmap);

        appWidgetManager.updateAppWidget(appWidgetID, views);
    }

    @Override
    public void onReceive(Context context, Intent intent)
    {
        super.onReceive(context, intent);
        RemoteViews views = new RemoteViews(context.getPackageName(), R.layout.android_widget);
        ComponentName appWidget = new ComponentName(context, AndroidWidget.class);
        AppWidgetManager appWidgetManager = AppWidgetManager.getInstance(context);

        if (ACTION_INCREMENT_1.equals(intent.getAction()))
        {
            AndroidUtils.incrementCounter(AndroidUtils.getWidgetName(0));
            views.setTextViewText(R.id.android_widget_count1, String.valueOf(AndroidUtils.getCountForName(AndroidUtils.getWidgetName(0))));

            AndroidUtils.updateWidgetChart();
            File chart0File = new File(context.getFilesDir().toString(), "chart0.png");
            Bitmap chart0Bitmap = BitmapFactory.decodeFile(chart0File.getAbsolutePath());
            views.setImageViewBitmap(R.id.android_widget_chart0, chart0Bitmap);
            File chart1File = new File(context.getFilesDir().toString(), "chart1.png");
            Bitmap chart1Bitmap = BitmapFactory.decodeFile(chart1File.getAbsolutePath());
            views.setImageViewBitmap(R.id.android_widget_chart1, chart1Bitmap);

            appWidgetManager.updateAppWidget(appWidget, views);
        }
        if (ACTION_INCREMENT_2.equals(intent.getAction()))
        {
            AndroidUtils.incrementCounter(AndroidUtils.getWidgetName(1));
            views.setTextViewText(R.id.android_widget_count2, String.valueOf(AndroidUtils.getCountForName(AndroidUtils.getWidgetName(1))));

            AndroidUtils.updateWidgetChart();
            File chart0File = new File(context.getFilesDir().toString(), "chart0.png");
            Bitmap chart0Bitmap = BitmapFactory.decodeFile(chart0File.getAbsolutePath());
            views.setImageViewBitmap(R.id.android_widget_chart0, chart0Bitmap);
            File chart1File = new File(context.getFilesDir().toString(), "chart1.png");
            Bitmap chart1Bitmap = BitmapFactory.decodeFile(chart1File.getAbsolutePath());
            views.setImageViewBitmap(R.id.android_widget_chart1, chart1Bitmap);

            appWidgetManager.updateAppWidget(appWidget, views);
        }
    }
}
