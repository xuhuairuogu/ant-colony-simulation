// System Requirements //

.NET Framework 4.0
Visual C++ 2012 Redistributable

// Simulation controls //

You can change various simulation parameters during execution to see effect in real time. Use mouse or keyboard to change parameter values.

// Configuration file //

Instead of changing all parameters in real-time, you can put them into configuration file and feed it to program on start-up. You can find example configuration file in the same folder as executable file. You can find description of all parameters in this file.

For simulation to use your configuration file you have to pass it as command line argument. For this you have to create shortcut for executable and inside target text box add name of your configuration file. Example “C:\FinalProject.exe input.xml”.

// About source building

This source has been tested and build on Visual Studio 2012 on both Windows 7 and Windows 8.

However, as project uses several 3rd party libraries it is possible that source might not build under other Visual Studio versions.

If you experience such problem you might to use Visual Studio 2012, or you have to rebuild all libraries inside your version of visual studio. This primary being GXBase (and maybe AntTweakBar).