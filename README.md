[![master](https://img.shields.io/badge/master-v1.0.0-blue.svg)](https://github.com/curzon01/lib_mysqludf_astro/tree/master)
[![License](https://img.shields.io/github/license/curzon01/lib_mysqludf_astro.svg)](LICENSE)

# MySQL sun/moon astronomy functions

This repository contains the source code for a [MySQL loadable function](https://dev.mysql.com/doc/extending-mysql/8.0/en/adding-loadable-function.html) library (previously called UDF - User Defined Functions), which provides some additonal SQL astrononmy functions to get sunrise, sunset, moon phase and others for a geographical location for a specified time.

This library is based on [ESP32-Astronomie](https://github.com/schreibfaul1/ESP32-Astronomie) implementation.

If you like **lib_mysqludf_astro** give it a star or fork it:

[![GitHub stars](https://img.shields.io/github/stars/curzon01/lib_mysqludf_astro.svg?style=social&label=Star)](https://github.com/curzon01/lib_mysqludf_astro/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/curzon01/lib_mysqludf_astro.svg?style=social&label=Fork)](https://github.com/curzon01/lib_mysqludf_astro/network)

## Build instructions for GNU Make

### Build and install

From the base directory run:

```bash
make && sudo make install
```

This will build and install the library file. 

To active the loadable function within your MySQL server run the follwoing SQL queries:

```SQL
CREATE FUNCTION astro_info RETURNS STRING SONAME 'lib_mysqludf_astro.so';
CREATE FUNCTION astro RETURNS STRING SONAME 'lib_mysqludf_astro.so';
```

### Uninstall

To uninstall first deactive the loadable function within your MySQL server running the SQL queries:

```SQL
DROP FUNCTION IF EXISTS astro_info;
DROP FUNCTION IF EXISTS astro;
```

then uninstall the library file using command line:

```bash
sudo make uninstall
```

# Usage

## astro(date, timezone)

Time
Zone
Latitude
Longitude
deltaT
JulianDate
GMST
LMST

Sun
Distance to the sun (Earth's center) in km
Distance to the sun (from the observer) in km
Ecliptic length of the sun in degree
Right ascension of the sun in degree
Declination of the sun in degree
Azimuth of the sun in degree
Height of the sun above the horizon in degree
Diameter of the sun in '

Sunrise
Astronomical dawn
Nautical dawn
Civil dawn
Culmination of the sun

Sunset
Civil dusk
Nautical dusk
Astronomical dusk
Signs of the zodiac

Distance to the moon (Earth's center) in km
Distance to the moon (from the observer) in km
Ecliptic longitude of the moon in degree
Ecliptic latitude of the moon in degree
Right ascension of the moon
Declination of the moon in degree
Azimuth of the moon in degree
Height of the moon above the horizon in degree
Diameter of the moon in '
Moonrise
Culmination of the moon
Moonset
Moon phase
Moon phase number
Mon age in degree
Mon sign


## astro_info()

Returns library info as JSON string

Examples:

```sql
> SELECT 
    JSON_UNQUOTE(JSON_VALUE(astro_info(),'$.Name')) AS Name,
    JSON_UNQUOTE(JSON_VALUE(astro_info(),'$."Version"')) AS Version,
    JSON_UNQUOTE(JSON_VALUE(astro_info(),'$."Build"')) AS Build;
+--------------------+---------+----------------------+
| Name               | Version | Build                |
+--------------------+---------+----------------------+
| lib_mysqludf_astro | 1.0.0   | Jan 16 2023 09:00:00 |
+--------------------+---------+----------------------+
