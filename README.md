# HexStartup--

A better, cross-platform tool to simply turn a .ppm file into a STARTUP, NOTCH
and NETNOTCH lump for the startup screen in Hexen, its loading bar, and its
network loading bar, respectively.

Everything here is dirty and chipped to bits in order to barely get it working.
Expect nothing short of quick hacks, memory errors and segmentation faults
if anything unexpected happens.

The output is currently written to STARTUP.DAT, NOTCH.DAT and NETNOTCH.DAT
in the working directory; this is coded in and cannot be changed on the fly
currently. It's planned to add better argument handling and multiple file
conversion (e.g., all three lumps) in the future, but it works enough for
me, for now.

The output files are WAD format LUMPs, and can be integrated to a WAD
using tools such as SLADE or DEUTEX, or a number of WAD packers/unpackers,
DOOM editors, and integrated mapping environments.

This tool takes one argument, which is the source PPM image to convert.

```hexstartup ~/path/to/file.ppm```

The file must be reduced to 16 colors, at a size of exactly 640x480 for
the STARTUP screen, 16x23 for NOTCH, and 4x16 for NETNOTCH.

This tool has fewer features than its predecessor,
[HexStartup++](https://github.com/ibm5155/HexStartup-)
and that is a *good thing.*

PNG support may come at a later date, but if you must have a PNG to work with

The STARTUP lump is a 16-color (4-bit per pixel) indexed-color planar VGA 
640x480 graphic, where the first 48 bytes contain the color definitions,
three bytes per color.

The NOTCH graphics are 16-color (4bpp) raw bitmaps, 16x23 pixels for NOTCH,
and 4x16 for NETNOTCH.

The original announcement post is at: https://forum.zdoom.org/viewtopic.php?f=44&t=56556

This is chopped down to mostly obey POSIX interfaces.
As such, it should be portable to Linux, UNIX, Win32 console, and Mac OS X.
