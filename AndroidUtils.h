#pragma once

#include <QObject>
#include <QtAndroidExtras>

#include "Global.h"
#include "Settings.h"
#include "Style.h"

#define ANDROID_UTILS_CLASS_NAME "com/WhenInDoubtC4/ApologyCounter/AndroidUtils"

class AndroidUtils : public QObject
{
	Q_OBJECT
public:
	static AndroidUtils* getInstance();

	void displayNewCounterMessageBox();
	void displayDeleteCounterMessageBox(QString displayName);
	void requestUpdateWidget();

private:
	AndroidUtils();

	inline static AndroidUtils* _instance = nullptr;

	QAndroidJniObject _androidUtilsJavaClass = QAndroidJniObject(ANDROID_UTILS_CLASS_NAME);

	static void getNewCounter(JNIEnv* env, jobject obj, jstring name);
	static void requestDeleteCounter(JNIEnv* env, jobject obj);
	static jobject getAndroidActivity(JNIEnv* env, jobject obj);
	static jstring getWidgetName(JNIEnv* env, jobject obj, jint index);
	static jstring getWidgetDisplayName(JNIEnv* env, jobject obj, jint index);
	static jint getCountForName(JNIEnv* env, jobject obj, jstring name);
	static void incrementCounter(JNIEnv* env, jobject obj, jstring name);
	static void updateWidgetChart(JNIEnv* env, jobject obj);
	static jint getStyleIndex(JNIEnv* env, jobject obj);
	static jstring getSecondaryColorHex(JNIEnv* env, jobject obj);

signals:
	void createNewCounter(QString name);
	void deleteCounter();
	void requestIncrementCounter(const QString name);

	void setStatsName(const QString& name);
	void saveWidgetChart(const QString& path);
};

