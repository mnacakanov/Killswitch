Simple KillSwitch application
Qt version 5.9

Application requires Administrator privileges to function properly.
Connection to WMI is performed with Active Qt (for COM support).

Active connection is achieved by filtering Win32_NetworkAdapter instances by NetConnectionStatus parameter
(NetConnectionStatus = 2 means adapter is connected to internet).
Disabling/Enabling network adapter is achieved by calling Win32_NetworkAdapter method Disable()/Enable(),
since disabling network adapter is more obvious way to achieve killswitch functionality than nullifying IP route tables From user perspective.

UI is based on QWidget. Check Box represents the state of killswitch.
In case there's no active internet connection on application startup,
application will inform user about that and close, since there is no action to perform.
