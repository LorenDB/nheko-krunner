# nheko-krunner

A KRunner plugin to list joined rooms and possibly other things from [nheko](https://nheko-reborn.github.io).

This project was initially based off of the template project found [here](https://invent.kde.org/frameworks/krunner/-/tree/master/templates/runner).

### Build instructions

```bash
mkdir build
cd build
cmake -DKDE_INSTALL_PLUGINDIR=`kf5-config --qt-plugins` ..
make
make install
kquitapp5 krunner
```

However, running `./install.sh` will do more or less the exact same thing with only one command.

Theoretically, KRunner will automatically relaunch itself and pick up on the new plugin.
