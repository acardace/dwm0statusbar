dwm0statusbar
=============
##Description
Dwm dynamic status bar daemon, done to help writing battery uneager status updater clients.

It just listens on a unix daemon (using datagrams) in /tmp called "dwm_statusbar" and prints whatever is passed into it.

I've done this as to have a simple server which eases the task writing of clients which want to print on dwm status bar with a minimum battery impact (using inotify for example).

There's a simple client named "dwm0c" included to test the server.

##Launch it!
just type `dwm0d &`

##ArchLinux package
A package called dwm0statusbard-git is available in the AUR.

### Thanks to
* github user: johnko (from whom I obtained the code that I eventually edited)

------------------------------------------------------------
Copyright **Antonio Cardace** 2015, cardace@tutanota.com
