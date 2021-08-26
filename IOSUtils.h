#pragma once

#include <QObject>

class IOSUtils : public QObject
{
	Q_OBJECT
public:
    static IOSUtils* getInstance();
    
    enum ImpactFeedbackStyle
    {
        LIGHT,
        SOFT,
        MEDIUM,
        RIGID,
        HEAVY
    };
    
    enum NotificationFeedbackType
    {
        SUCCESS,
        WARNING,
        ERROR
    };

    static void playImpactHapticFeedback(ImpactFeedbackStyle style);
    static void playSelectionHapticFeedback();
    static void playNotificationHapticFeedback(NotificationFeedbackType type);
    void displayNewCounterMessageBox();
    void displayDeleteCounterMessageBox(const QString& displayName, const QString& safeName);
    
private:
    IOSUtils();
    
    inline static IOSUtils* _instance = nullptr;
    
signals:
    void createNewCounter(QString name);
    void deleteCounter();
};

