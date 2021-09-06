#include "AndroidUtils.h"

AndroidUtils::AndroidUtils()
{
	JNINativeMethod methods[]
	{
		{"createNewCounter", "(Ljava/lang/String;)V", reinterpret_cast<void*>(getNewCounter)},
		{"deleteCounter", "()V", reinterpret_cast<void*>(requestDeleteCounter)},
		{"getAndroidActivity", "()Landroid/app/Activity;", reinterpret_cast<jobject*>(getAndroidActivity)},
		{"getWidgetName", "(I)Ljava/lang/String;", reinterpret_cast<jstring*>(getWidgetName)},
		{"getWidgetDisplayName", "(I)Ljava/lang/String;", reinterpret_cast<jstring*>(getWidgetDisplayName)},
		{"getCountForName", "(Ljava/lang/String;)I", reinterpret_cast<jint*>(getCountForName)},
		{"incrementCounter", "(Ljava/lang/String;)V", reinterpret_cast<void*>(incrementCounter)},
		{"updateWidgetChart", "()V", reinterpret_cast<void*>(updateWidgetChart)},
		{"getStyleIndex", "()I", reinterpret_cast<jint*>(getStyleIndex)},
		{"getSecondaryColorHex", "()Ljava/lang/String;", reinterpret_cast<jstring*>(getSecondaryColorHex)}
	};

	QAndroidJniEnvironment env;
	jclass objectClass = env->GetObjectClass(_androidUtilsJavaClass.object<jobject>());
	env->RegisterNatives(objectClass, methods, sizeof(methods) / sizeof(methods[0]));
	env->DeleteLocalRef(objectClass);

	requestUpdateWidget();
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

void AndroidUtils::requestDeleteCounter([[maybe_unused]] JNIEnv* env, [[maybe_unused]] jobject obj)
{
	emit AndroidUtils::getInstance()->deleteCounter();
}

jobject AndroidUtils::getAndroidActivity(JNIEnv* env, [[maybe_unused]] jobject obj)
{
	return env->NewGlobalRef(QtAndroid::androidActivity().object());
}

jstring AndroidUtils::getWidgetName(JNIEnv* env, [[maybe_unused]] jobject obj, jint index)
{
	QString name = Settings::getWidgetName(static_cast<int>(index));
	return env->NewStringUTF(name.toUtf8());
}

jstring AndroidUtils::getWidgetDisplayName(JNIEnv* env, [[maybe_unused]] jobject obj, jint index)
{
	QString displayName = Settings::getWidgetDisplayName(static_cast<int>(index));
	return env->NewStringUTF(displayName.toUtf8());
}

void AndroidUtils::requestUpdateWidget()
{
	_androidUtilsJavaClass.callMethod<void>("requestUpdateWidget", "()V");
}

jint AndroidUtils::getCountForName(JNIEnv* env, [[maybe_unused]] jobject obj, jstring name)
{
	const QString safeName = QString::fromUtf8(env->GetStringUTFChars(name, nullptr));
	QSettings settings;
	const int count = settings.value(safeName + "/" DATA_COUNT, 0).toInt();
	return static_cast<jint>(count);
}

void AndroidUtils::incrementCounter(JNIEnv* env, [[maybe_unused]] jobject obj, jstring name)
{
	const QString safeName = QString::fromUtf8(env->GetStringUTFChars(name, nullptr));
	emit AndroidUtils::getInstance()->requestIncrementCounter(safeName);

	QSettings settings;
	settings.beginGroup(safeName);
	const int prevCount = settings.value(DATA_COUNT).toInt();
	settings.setValue(DATA_COUNT, prevCount + 1);
	const int size = settings.value(DATA_TIME_DATA "/size").toInt();
	settings.beginWriteArray(DATA_TIME_DATA);
	settings.setArrayIndex(size);
	settings.setValue(DATA_TIMESTAMP, QDateTime::currentDateTime().toString(Qt::ISODate));
	settings.endArray();
	settings.endGroup();
	settings.sync();
}

void AndroidUtils::updateWidgetChart([[maybe_unused]] JNIEnv* env, [[maybe_unused]] jobject obj)
{
	const QString filePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

	emit getInstance()->setStatsName(Settings::getWidgetName(0));
	emit getInstance()->saveWidgetChart(filePath + "/chart0.png");

	emit getInstance()->setStatsName(Settings::getWidgetName(1));
	emit getInstance()->saveWidgetChart(filePath + "/chart1.png");
}

jint AndroidUtils::getStyleIndex([[maybe_unused]] JNIEnv* env, [[maybe_unused]] jobject obj)
 {
	const int index = Style::getIndex();
	return static_cast<jint>(index);
 }

jstring AndroidUtils::getSecondaryColorHex(JNIEnv* env, [[maybe_unused]] jobject obj)
{
	const QString hex = Style::getSecondaryColor().name();
	return env->NewStringUTF(hex.toUtf8());
}
