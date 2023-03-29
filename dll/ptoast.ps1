#$OutputEncoding = [Text.UTF8Encoding]::UTF8
#$InputEncoding = [Text.UTF8Encoding]::UTF8
#write-host $args[0]
#write-host $args[1]

$t=$args[0]
$m=$args[1]
$l=[Windows.UI.Notifications.ToastNotificationManager,Windows.UI.Notifications,ContentType=WindowsRuntime]::GetTemplateContent([Windows.UI.Notifications.ToastTemplateType,Windows.UI.Notifications,ContentType=WindowsRuntime]::ToastText01)
$l.GetElementsByTagName('text').Item(0).InnerText=$m
[Windows.UI.Notifications.ToastNotificationManager]::CreateToastNotifier($t).Show($l)

