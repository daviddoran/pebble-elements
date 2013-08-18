# Chemical Elements Watch Face

This simple watch face shows the current hour and minute and the symbol and name of the chemical element with the atomic number of the current minute.

![Inverted Watch Face](http://daviddoran.github.io/pebble-elements/action-shot-inverted.jpg "Inverted Watch Face")

## Building From Source

First, clone the repository's files:

    git clone git://github.com/daviddoran/pebble-elements.git

Next, make the directory a Pebble project:

    export PEBBLE=~/pebble-dev/PebbleSDK-1.12/Pebble
    $PEBBLE/tools/create_pebble_project.py --symlink-only $PEBBLE/sdk pebble-elements

Enter the project directory:

    cd pebble-elements

Configure and build the watch face:

    ./waf configure && ./waf build

## Inverted Colours

For inverted colours (white on black) uncomment the following line in `periodic-elements.c`:

    #define INVERSED_COLORS

## License

This project is released under the MIT License - see the LICENSE file for details.
