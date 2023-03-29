
@echo off


if A%1 == A (
    SET title=No Title
) else (
    SET title=%1
)

if A%2 == A (
    SET msg=No Message
) else (
    SET msg=%2
)
@rem chcp 65001
@rem echo %title%
@rem echo %msg%
PowerShell.exe ^&{chcp 65001 > /nul;$t='%title%';$m='%msg%';$l=[Windows.UI.Notifications.ToastNotificationManager,Windows.UI.Notifications,ContentType=WindowsRuntime]::GetTemplateContent([Windows.UI.Notifications.ToastTemplateType,Windows.UI.Notifications,ContentType=WindowsRuntime]::ToastText01);$l.GetElementsByTagName('text').Item(0).InnerText=$m;[Windows.UI.Notifications.ToastNotificationManager]::CreateToastNotifier($t).Show($l);}
