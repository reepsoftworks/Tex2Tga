# Tex2Tga
Texture decompilation tool for Leadwerks/Ultra Engine.

An [Ultra Engine](https://www.ultraengine.com) project is provided, and a license of the SDK is required to build.

## Usage
You can easily decompile any texture by drag and drop. additionally, you can drop entire folders onto the application, and the program will convert all contents recursively.

For power users, the program can also be run via the command line.
```
./tex2tga "ClassicTextures/mytexture.tex" 
```
or
```
./tex2tga "ClassicTextures/" 
```

By default, the texture will be decompiled as a tga file. To convert it to a png file, you would do the following
```
./tex2tga "ClassicTextures/mytexture.tex" -png
```

The file can be placed within your Ultra Engine project or the Engine SDK's "Tools" directory.

## License
This code and application may be used freely to develop projects using Leadwerks and Ultra Engine. No other use is allowed. Do not mispresent the origin of this code.