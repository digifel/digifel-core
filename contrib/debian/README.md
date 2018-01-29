
Debian
====================
This directory contains files used to package digifeld/digifel-qt
for Debian-based Linux systems. If you compile digifeld/digifel-qt yourself, there are some useful files here.

## digifel: URI support ##


digifel-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install digifel-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your digifel-qt binary to `/usr/bin`
and the `../../share/pixmaps/digifel128.png` to `/usr/share/pixmaps`

digifel-qt.protocol (KDE)

