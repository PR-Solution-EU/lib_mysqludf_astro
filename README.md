[![master](https://img.shields.io/badge/master-v1.0.0-blue.svg)](https://github.com/curzon01/lib_mysqludf_astro/tree/master)
[![License](https://img.shields.io/github/license/curzon01/lib_mysqludf_astro.svg)](LICENSE)

# MySQL sun/moon astronomy functions

This repository contains the source code for a [MySQL loadable function](https://dev.mysql.com/doc/extending-mysql/8.0/en/adding-loadable-function.html) library (previously called UDF - User Defined Functions), which provides some additonal SQL astrononmy functions to get sunrise, sunset, moon phase and others for a geographical location for a specified time.

This library is based on [ESP32-Astronomie](https://github.com/schreibfaul1/ESP32-Astronomie) implementation.

If you like **lib_mysqludf_astro** give it a star or fork it:

[![GitHub stars](https://img.shields.io/github/stars/curzon01/lib_mysqludf_astro.svg?style=social&label=Star)](https://github.com/curzon01/lib_mysqludf_astro/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/curzon01/lib_mysqludf_astro.svg?style=social&label=Fork)](https://github.com/curzon01/lib_mysqludf_astro/network)

## Build instructions for GNU Make

Ensure the [Eclipse Paho C Client Library for the MQTT Protocol](https://github.com/eclipse/paho.mqtt.c) is installed.<br>
Also install libjsonparser:

```bash
sudo apt install libjsonparser-dev
```

### Install

From the base directory run:

```bash
make && sudo make install
```

This will build and install the library file. 

To active the loadable function within your MySQL server run the follwoing SQL queries:

```SQL
CREATE FUNCTION astro_info RETURNS STRING SONAME 'lib_mysqludf_astro.so';
```

### Uninstall

To uninstall first deactive the loadable function within your MySQL server running the SQL queries:

```SQL
DROP FUNCTION IF EXISTS astro_info;
```

Then uninstall the library file using command line:

```bash
sudo make uninstall
```

# Usage

## MySQL loadable functions
