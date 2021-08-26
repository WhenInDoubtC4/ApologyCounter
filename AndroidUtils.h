#pragma once

#include <QObject>
#include <QtAndroidExtras>

#define ANDROID_UTILS_CLASS_NAME "com/WhenInDoubtC4/ApologyCounter/AndroidUtils"

class AndroidUtils : public QObject
{
	Q_OBJECT
public:
	static AndroidUtils* getInstance();

	void displayNewCounterMessageBox();
	void displayDeleteCounterMessageBox(QString displayName);

private:
	AndroidUtils();

	inline static AndroidUtils* _instance = nullptr;

	QAndroidJniObject _androidUtilsJavaClass = QAndroidJniObject(ANDROID_UTILS_CLASS_NAME);

	static void getNewCounter(JNIEnv* env, jobject obj, jstring name);
	static void requestDeleteCounter(JNIEnv* env, jobject obj);

signals:
	void createNewCounter(QString name);
	void deleteCounter();
};

