package com.WhenInDoubtC4.ApologyCounter;

import android.annotation.TargetApi;
import android.app.PendingIntent;
import android.appwidget.AppWidgetManager;
import android.appwidget.AppWidgetProvider;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.widget.RemoteViews;

@TargetApi(Build.VERSION_CODES.CUPCAKE)
public class AndroidWidget extends AppWidgetProvider
{
    private static final String ACTION_INCREMENT_1 = "ACTION_INCREMENT_1";

    protected static PendingIntent getPendingSelfIntent(Context context, String action)
    {
        Intent intent = new Intent(context, AndroidWidget.class);
        intent.setAction(action);
        return PendingIntent.getBroadcast(context, 0, intent, PendingIntent.FLAG_UPDATE_CURRENT);
    }

    @Override
    public void onAppWidgetOptionsChanged(Context context, AppWidgetManager appWidgetManager, int appWidgetId, Bundle newOptions)
    {
        int minWidth = newOptions.getInt(AppWidgetManager.OPTION_APPWIDGET_MIN_WIDTH);
        int minHeight = newOptions.getInt(AppWidgetManager.OPTION_APPWIDGET_MIN_HEIGHT);
        int maxWidth = newOptions.getInt(AppWidgetManager.OPTION_APPWIDGET_MAX_WIDTH);
        int maxHeight = newOptions.getInt(AppWidgetManager.OPTION_APPWIDGET_MAX_HEIGHT);

        updateWidget(context, appWidgetManager, appWidgetId);
    }

    @Override
    public void onUpdate(Context context, AppWidgetManager appWidgetManager, int[] appWidgetIds)
    {
        for (int appWidgetID : appWidgetIds) updateWidget(context, appWidgetManager, appWidgetID);
    }

    private static void updateWidget(Context context, AppWidgetManager appWidgetManager, int appWidgetID)
    {
        RemoteViews views = new RemoteViews(context.getPackageName(), R.layout.android_widget);
        views.setTextViewText(R.id.android_widget_name1, AndroidUtils.getWidgetDisplayName(0));
        views.setTextViewText(R.id.android_widget_count1, String.valueOf(AndroidUtils.getCountForName(AndroidUtils.getWidgetName(0))));
        views.setOnClickPendingIntent(R.id.android_widget_increment1, getPendingSelfIntent(context, ACTION_INCREMENT_1));
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
            //views.setTextViewText(R.id.android_widget_count1, "69");
            AndroidUtils.incrementCounter(AndroidUtils.getWidgetName(0));
            views.setTextViewText(R.id.android_widget_count1, String.valueOf(AndroidUtils.getCountForName(AndroidUtils.getWidgetName(0))));
            appWidgetManager.updateAppWidget(appWidget, views);
        }
    }
}
