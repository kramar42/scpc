# prerequisites

`python3` & `pip` to install `glad` package

on windows: command line with `vcvars64.bat` to run `msbuild.exe`, `cl.exe` & `link.exe`

on linux/macos: `cc`, `pkg-config` & `glfw3` library

# prepare

build process relies on glfw distribution which you can get at https://github.com/glfw/glfw

on windows: run `install_deps %PATH_TO_GLFW%` to prepare for building

on linux/macos: `./install_deps.sh`

# build

on windows: `build`

on linux/macos: `./build.sh`

# run

output is stored in `build`
