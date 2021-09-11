QT += core quick widgets charts svg

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Counter.cpp \
        CounterManager.cpp \
        Settings.cpp \
        Stats.cpp \
        Style.cpp \
        main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
	Counter.h \
	CounterManager.h \
	Global.h \
	Settings.h \
	Stats.h \
	Style.h

ios {
	OBJECTIVE_SOURCES += \
			IOSUtils.mm

	HEADERS += \
			IOSUtils.h
}

android {
	QT += androidextras

	SOURCES += \
			AndroidUtils.cpp

	HEADERS += \
			AndroidUtils.h
}

macos {
	ICON = Resources/macIcon.icns
}

OTHER_FILES += \
	android/AndroidManifest.xml \
	android/build.gradle \
	android/gradle.properties \
	android/gradle/wrapper/gradle-wrapper.jar \
	android/gradle/wrapper/gradle-wrapper.properties \
	android/gradlew \
	android/gradlew.bat \
	android/res/values/libs.xml \
	android/src/com/WhenInDoubtC4/ApologyCounter/AndroidUtils.java \
	android/src/com/WhenInDoubtC4/ApologyCounter/AndroidWidget.java

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

DISTFILES += \
	.gitignore
