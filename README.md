# FX Studio

![Editor screenshot](https://github.com/chenjinxian/FX-Studio/blob/master/Build/editor_screenshot.png)

## Getting up and running

The steps below will take you through cloning your own private fork, then compiling and running the studio yourself:

### Windows

1. Install **[GitHub for Windows](https://windows.github.com/)** then **[fork and clone our repository](https://guides.github.com/activities/forking/)**. 
   To use Git from the command line, see the [Setting up Git](https://help.github.com/articles/set-up-git/) and [Fork a Repo](https://help.github.com/articles/fork-a-repo/) articles.

   If you'd prefer not to use Git, you can get the source with the 'Download ZIP' button on the right. The built-in Windows zip utility will mark the contents of zip files 
   downloaded from the Internet as unsafe to execute, so right-click the zip file and select 'Properties...' and 'Unblock' before decompressing it.

1. Install **Visual Studio 2015**. 
   All desktop editions of Visual Studio 2015 can build FX-Studio, including [Visual Studio Community 2015](http://www.visualstudio.com/products/visual-studio-community-vs), which is free for small teams and individual developers.
   Be sure to include C++ support as part of the install, which is disabled by default.
   
1. Download boost c++ library, here we use the [boost 1.61.0](https://sourceforge.net/projects/boost/files/boost/1.61.0/), then decompressing it to **ThirdParty\boost** directory. Make sure all boost include file are located at **ThirdParty\boost\boost** folder.
  
1. Load the project into Visual Studio by double-clicking on the **Build\FXStudio.sln** file. Set your solution configuration to **Debug** and your solution
   platform to **x64**, then right click on the **FXStudio** target and select **Build**. It may take less than one minutes to finish compiling, depending on your system specs.

1. After compiling finishes, you can load the studio from Visual Studio by setting your startup project to **FXStudio** and pressing **F5** to debug.

## [Examples](https://github.com/chenjinxian/FX-Studio-Examples)

All examples of FX Studio.

### [Materials](https://github.com/chenjinxian/FX-Studio-Examples)

![Materials screenshot](https://github.com/chenjinxian/FX-Studio-Examples/blob/master/Materials.png)

### [VisualStyles](https://github.com/chenjinxian/FX-Studio-Examples)

![VisualStyles screenshot](https://github.com/chenjinxian/FX-Studio-Examples/blob/master/VisualStyles.png)
