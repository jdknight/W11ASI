# W11ASI

[![Build Status](https://github.com/jdknight/w11asi/actions/workflows/build.yml/badge.svg)](https://github.com/jdknight/w11asi/actions/workflows/build.yml)

## Overview

Provides the ability in Windows 11 to help force the promotion of all system
icons in the system tray.

This application was created with the goal of supporting the removed
functionality of Window's "Always show all icons in the notification area".
When this utility is run, it will first ensure all known system icon entries
are visible. Afterwards, it will monitor for changes to the system tray (such
as new/updated applications) and ensure they remain visible.

## Install

1. Extract the `w11asi-portable.zip` package and place the contents in
   any desired directory (e.g. `C:\w11asi`).
2. *(optional)* Run the `install.bat` script to have the program start after
   logging in.
3. Run `w11asi.exe`, which will run in the background.

Users can also use `shutdown.bat` to terminate the program (or use the task
manager, if preferred). Also, an `uninstall.bat` script is provided to
unregister the application from starting when the user logs in.
