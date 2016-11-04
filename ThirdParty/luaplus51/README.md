# Building LuaPlus for Windows

## Prerequisites

* Download _Wget_. One location to grab it from is [http://users.ugent.be/~bpuype/wget/](http://users.ugent.be/~bpuype/wget/). Put `wget.exe` in your `PATH`.
* Download _unzip_ from [http://www.info-zip.org/UnZip.html](http://www.info-zip.org/UnZip.html). Put `unzip.exe` in your `PATH`.

For Visual Studio:

* From [http://www.npcglib.org/~stathis/blog/precompiled-openssl/](http://www.npcglib.org/~stathis/blog/precompiled-openssl/), download **openssl-1.0.2f-vs20xx.7z** (where the 'xx' is replaced with your version of Visual Studio). Extract it to `c:\openssl-1.0.2f-vs20xx`, so that `c:\openssl-1.0.2f-vs20xx\include` exists.

(deprecated) For earlier versions of Visual Studio:

* From [https://slproweb.com/products/Win32OpenSSL.html](https://slproweb.com/products/Win32OpenSSL.html), download **Win32 OpenSSL v1.0.2g**. Install `Win32OpenSSL-1_0_2g.exe` to `c:\OpenSSL-Win32`.
* From [https://slproweb.com/products/Win32OpenSSL.html](https://slproweb.com/products/Win32OpenSSL.html), download **Win64 OpenSSL v1.0.2g**. Install `Win64OpenSSL-1_0_2g.exe` to `c:\OpenSSL-Win64`.

## Build LuaPlus

* If you haven't already, clone LuaPlus from GitHub with:

<pre>
d:\>git clone https://github.com/jjensen/luaplus51-all.git luaplus
</pre>

* To create a workspace and bootstrap the build environment, run one of the following batch files.
	* `CreateVS2010Workspace.bat` - Create a Visual Studio 2010-capable workspace.
	* `CreateVS2012Workspace.bat` - Create a Visual Studio 2012-capable workspace.
	* `CreateVS2013Workspace.bat` - Create a Visual Studio 2013-capable workspace.
	* `CreateVS2015Workspace.bat` - Create a Visual Studio 2015-capable workspace.

* Choose the version of Lua to build within the workspace.  Note that the **-luaplus** versions build additional LuaPlus-specific features into the Lua environment; these are the preferred environments for LuaPlus.

* If you want lua-curl support, run the following (where build201x is your own build directory):

<pre>
d:\luaplus\build201x>jam download:curl
</pre>

* If you want the PCRE module to build, run the following (where build201x is your own build directory):

<pre>
d:\luaplus\Src\modules.jambuild>getpcre.bat
</pre>

If you want to build from an IDE:

* Within Visual Studio, navigate to `build201x\_workspace_\vs201x\` (replace the `vs201x` with the IDE version your are building for) and open `LuaPlus.sln`.
* Build the solution.

If you want to build from the command-line, use:

<pre>
rem Builds to d:\luaplus\build201x\bin.lua5x-luaplus.vs20xx.win32
d:\luaplus\build201x>jam c.toolchain=win32/release

rem Builds to d:\luaplus\build201x\bin.lua5x-luaplus.vs20xx.win32
d:\luaplus\build201x>jam c.toolchain=win32/debug

rem Builds to d:\luaplus\build201x\bin.lua5x-luaplus.vs20xx.win64
d:\luaplus\build201x>jam c.toolchain=win64/release

rem Builds to d:\luaplus\build201x\bin.lua5x-luaplus.vs20xx.win64
d:\luaplus\build201x>jam c.toolchain=win64/debug
</pre>
