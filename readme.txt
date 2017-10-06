Second Iteration of Killswitch application
Qt version 5.9

Application requires Administrator privileges to function properly.
Connection to WMI is performed with Active Qt (for COM support).

Active connection is achieved by filtering Win32_NetworkAdapter instances by NetConnectionStatus parameter
(NetConnectionStatus = 2 means adapter is connected to internet).
Disabling network adapter is achieved by deleting all instances of Win32_IP4RouteTable (deleting ip route table).
Enabling network adapter is achieved by invoking ReleaseDHCPLease method and then invoking RenewDHCPLease on active Win32_NetworkAdapterConfiguration, because simply restoring ip route table doesn't help.

Application has QWebengine based ui. Check Box represents the state of killswitch and is duplicated by textual label, which also signalise about killswitch's change of state.
In case there's no active internet connection on application startup,
application will inform user about that and close, since there is no action to perform.
