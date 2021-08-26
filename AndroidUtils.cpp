#include "AndroidUtils.h"

#include <QDebug>

AndroidUtils::AndroidUtils()
{
	JNINativeMethod methods[]
	{
		{"createNewCounter", "(Ljava/lang/String;)V", reinterpret_cast<void*>(getNewCounter)},
		{"deleteCounter", "()V", reinterpret_cast<void*>(requestDeleteCounter)}
	};

	QAndroidJniEnvironment env;
	jclass objectClass = env->GetObjectClass(_androidUtilsJavaClass.object<jobject>());
	env->RegisterNatives(objectClass, methods, sizeof(methods) / sizeof(methods[0]));
	env->DeleteLocalRef(objectClass);
}

AndroidUtils* AndroidUtils::getInstance()
{
	if (!_instance) _instance = new AndroidUtils();
	return _instance;
}

void AndroidUtils::displayNewCounterMessageBox()
{
	_androidUtilsJavaClass.callMethod<void>("displayNewCounterMessageBox", "(Landroid/app/Activity;)V",  QtAndroid::androidActivity().object());
}

void AndroidUtils::displayDeleteCounterMessageBox(QString displayName)
{
	QAndroidJniObject javaDisplayName = QAndroidJniObject::fromString(displayName);
	_androidUtilsJavaClass.callMethod<void>("displayDeleteCounterMessageBox", "(Landroid/app/Activity;Ljava/lang/String;)V", QtAndroid::androidActivity().object(), javaDisplayName.object<jstring>());
}

void AndroidUtils::getNewCounter(JNIEnv* env, [[maybe_unused]] jobject obj, jstring name)
{
	const QString displayName = QString::fromUtf8(env->GetStringUTFChars(name, nullptr));
	emit AndroidUtils::getInstance()->createNewCounter(displayName);
}

void AndroidUtils::requestDeleteCounter([[maybe_unused]] JNIEnv *env, [[maybe_unused]] jobject obj)
{
	qDebug() << "Deleting counter from c++";
	emit AndroidUtils::getInstance()->deleteCounter();
}
