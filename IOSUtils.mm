#include "IOSUtils.h"

#include <UIKit/UIImpactFeedbackGenerator.h>
#include <UIKit/UISelectionFeedbackGenerator.h>
#include <UIKit/UINotificationFeedbackGenerator.h>
#include <UIKit/UIAlertController.h>
#include <UIKit/UIWindow.h>
#include <UIKit/UITextField.h>
#include <UIKit/NSAttributedString.h>

IOSUtils::IOSUtils()
{

}

IOSUtils* IOSUtils::getInstance()
{
    if (!_instance) _instance = new IOSUtils();
    return _instance;
}

void IOSUtils::playImpactHapticFeedback(ImpactFeedbackStyle style)
{
    auto feedbackGenerator = [[UIImpactFeedbackGenerator alloc] init];
    
    switch (style)
    {
        case IOSUtils::ImpactFeedbackStyle::LIGHT:
            [feedbackGenerator initWithStyle:UIImpactFeedbackStyleLight];
            break;
        case IOSUtils::ImpactFeedbackStyle::SOFT:
            [feedbackGenerator initWithStyle:UIImpactFeedbackStyleSoft];
            break;
        case IOSUtils::ImpactFeedbackStyle::MEDIUM:
            [feedbackGenerator initWithStyle:UIImpactFeedbackStyleMedium];
            break;
        case IOSUtils::ImpactFeedbackStyle::RIGID:
            [feedbackGenerator initWithStyle:UIImpactFeedbackStyleRigid];
            break;
        case IOSUtils::ImpactFeedbackStyle::HEAVY:
            [feedbackGenerator initWithStyle:UIImpactFeedbackStyleHeavy];
            break;
    }
    
    [feedbackGenerator impactOccurred];
}

void IOSUtils::playSelectionHapticFeedback()
{
    auto feedbackGenerator = [[UISelectionFeedbackGenerator alloc] init];
    [feedbackGenerator prepare];
    [feedbackGenerator selectionChanged];
}

void IOSUtils::playNotificationHapticFeedback(NotificationFeedbackType type)
{
    auto feedbackGenerator = [[UINotificationFeedbackGenerator alloc] init];
    [feedbackGenerator prepare];
    
    switch (type)
    {
        case IOSUtils::NotificationFeedbackType::SUCCESS:
            [feedbackGenerator notificationOccurred:UINotificationFeedbackTypeSuccess];
            break;
        case IOSUtils::NotificationFeedbackType::WARNING:
            [feedbackGenerator notificationOccurred:UINotificationFeedbackTypeWarning];
            break;
        case IOSUtils::NotificationFeedbackType::ERROR:
            [feedbackGenerator notificationOccurred:UINotificationFeedbackTypeError];
            break;
    }
}

void IOSUtils::displayNewCounterMessageBox()
{
    __block UITextField* messageBoxTextField = nullptr;
    
    UIAlertController* messageBox = [UIAlertController alertControllerWithTitle:@"New counter" message:@"Type the name of the person who apologizes a lot" preferredStyle:UIAlertControllerStyleAlert];
    UIAlertAction* cancelAction = [UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel handler:^([[maybe_unused]] UIAlertAction* action){}];
    UIAlertAction* defaultAction = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:^([[maybe_unused]] UIAlertAction* action)
    {
        const char* name = [[messageBoxTextField text] cStringUsingEncoding:NSUTF8StringEncoding];
        emit createNewCounter(QString::fromUtf8(name));
    }];
    [messageBox addAction:cancelAction];
    [messageBox addAction:defaultAction];
    [messageBox addTextFieldWithConfigurationHandler:^(UITextField* textField)
    {
        textField.placeholder = @"Name";
        messageBoxTextField = textField;
    }];
    [[[[UIApplication sharedApplication] keyWindow] rootViewController] presentViewController:messageBox animated:YES completion:nil];
}

void IOSUtils::displayDeleteCounterMessageBox(const QString &displayName, const QString& safeName)
{
    NSString* name = [NSString stringWithUTF8String:displayName.toUtf8()];
    NSString* message =[NSString stringWithFormat:@"Are you sure you want to delete the counter for %@? This will delete all the stats and data!", name];
    NSMutableAttributedString* formattedMessage = [[NSMutableAttributedString alloc] initWithString:message];
    [formattedMessage addAttribute:NSFontAttributeName value:[UIFont systemFontOfSize:[UIFont systemFontSize]] range:NSMakeRange(0, message.length)];
    [formattedMessage addAttribute:NSFontAttributeName value:[UIFont boldSystemFontOfSize:[UIFont systemFontSize]] range:[message rangeOfString:name]];
    
    UIAlertController* messageBox = [UIAlertController alertControllerWithTitle:@"Delete counter" message:nil preferredStyle:UIAlertControllerStyleAlert];
    [messageBox setValue:formattedMessage forKey:@"attributedMessage"];
    UIAlertAction* cancelAction = [UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel handler:^([[maybe_unused]] UIAlertAction* action){}];
    UIAlertAction* deleteAction = [UIAlertAction actionWithTitle:@"Delete" style:UIAlertActionStyleDestructive handler:^([[maybe_unused]] UIAlertAction* action)
    {
        emit deleteCounter();
    }];
    [messageBox addAction:cancelAction];
    [messageBox addAction:deleteAction];
    [[[[UIApplication sharedApplication] keyWindow] rootViewController] presentViewController:messageBox animated:YES completion:nil];
}
