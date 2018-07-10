# HexStartup--

A better, cross-platform tool to simply turn a .ppm file into a STARTUP, NOTCH
and NETNOTCH lump for the startup screen in Hexen, its loading bar, and its
network loading bar, respectively.

Everything here is dirty and chipped to bits in order to barely get it working.
Expect nothing short of quick hacks, memory errors and segmentation faults
if anything unexpected happens. It is only just minimally tested and Works On
My Machine™. Do not feed it edge cases.

This tool has fewer features than its predecessor,
[HexStartup++](https://github.com/ibm5155/HexStartup-)
and that is a *good thing.*

PNG support may come back at a later date, but if you must have a more 
common image format to work with, feel free to use the ImageMagick `convert`
command, or export it from GIMP.

# Usage

This tool takes one argument, which is the source PPM image to convert.

```hexstartup ~/path/to/file.ppm```

The input file must be reduced to 16 colors, at a size of exactly 640x480
for the STARTUP screen, 16x23 for NOTCH, and 4x16 for NETNOTCH.

The output is currently written to STARTUP.DAT, NOTCH.DAT or NETNOTCH.DAT
in the working directory; this is coded in and cannot be changed on the fly
currently. It's planned to add better argument handling and multiple file
conversion (e.g., all three lumps) in the future, but it works enough for
me, for now.

The output files are WAD format LUMPs, and can be integrated to a WAD
using tools such as SLADE or DEUTEX, or a number of WAD packers/unpackers,
DOOM editors, and integrated mapping environments.

# Building

No fancy makefile or cmake yet. Just run

```g++ -o hexstartup hexen.cpp image.cpp ppm.cpp main.cpp```

There shouldn't be any GNUisms in there - feel free to substitute your favorite
compiler/linker.

# File format info

The STARTUP lump is a 16-color (4-bit per pixel) indexed-color planar VGA 
640x480 graphic, where the first 48 bytes contain the color definitions,
three bytes per color; the remainder is the entire screen. Total file size
should be 153648 bytes.

The screen area of (64,441) to (576,464 or 457) is overdrawn by the status bar.

The NOTCH graphics are 16-color (4bpp) raw bitmaps, 16x23 pixels for NOTCH,
and 4x16 for NETNOTCH.


# Original source and attributions

The original announcement post is at: https://forum.zdoom.org/viewtopic.php?f=44&t=56556

This is chopped down to mostly obey POSIX interfaces.
As such, it should be portable to Linux, UNIX, Win32 console, and Mac OS X.

This code is used, WITHOUT permission, under no license from the original author.

The original code contained zlib/libpng libraries; these have been necessarily
removed from this fork, as it is not needed.

It is believed that the use of this code is in good faith in this project.

The original repository by ibm5155 is at https://github.com/ibm5155/HexStartup-
