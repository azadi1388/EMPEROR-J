# EMPEROR-J
EMPEROR Junior team 

![team_logo](https://user-images.githubusercontent.com/120306894/235450917-78fd61bf-3502-4868-b8bc-d5519304bca0.png)

![License](https://img.shields.io/github/license/ERFANFATHi-E/EMPEROR-J?color=blue&style=for-the-badge)

It is a team for the RoboCup Soccer 2D Simulator.
All programs can work with rcssserver-16 or later.

- The RoboCup Soccer Simulator: http://github.com/rcsoccersim/
- RoboCup Official Homepage: http://www.robocup.org/

## :soccer: Quick Start
-----------------------
The latest EMPEROR-J depends on the following libraries:
 - Boost 1.38 or later https://www.boost.org/
 - The latest librcsc https://github.com/helios-base/librcsc

In the case of Ubuntu 16.04 or later, execute the following commands for installing a basic development environment:
```
sudo apt update
sudo apt install build-essential libboost-all-dev
```
And, install librcsc.

To build binaries, execute commands from the root of source directory:
```
./bootstrap
./configure
make
```

To start the team, invoke the start script in `src` directory.
```
./start.sh
```

## :gear: Configuring

You may need to configure the build settings if you installed librcsc to an unusual location. 
You can specify the installed location of librcsc by using `--with-librcsc` option as follows:
```
./configure --with-librcsc=/path/to/librcsc
```
Usually, this value is the same as `--prefix` option of configuring librcsc.
You may also need to set the environment variable `LD_LIBRARY_PATH`.

See `./configure --help` for others options.

## :book: References

The paper about EMPEROR-J:
- Erfan Fathi , Soroush Mazloum , and Taha shahsavand: EMPEROR-J team



##

You can contact us at EmperorTmRCSS@gmail.com
