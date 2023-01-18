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
make clean
make
sudo make install
```

This will build and install the library file.

#### Localized binary

To create a country-specific version, call make command with the parameter LANG=-DLANG_xx, where xx is the country code: DE=German, ES=Spanish, FR=French, IT=Italian, NL=Netherlands, EN=English (default).

Example: Create and install a Dutch binary use
```bash
make clean
make LANG=-DLANG_NL
sudo make install
```

Finally we activate the loadable function in MySQL Server (replace `username` by a local MySQL user which has the permission to create functions, e. g. root)

```bash
mysql -u username -p < install.sql
```

### Uninstall

To uninstall first deactive the loadable function within your MySQL server using the SQL queries:

```SQL
DROP FUNCTION IF EXISTS astro_info;
DROP FUNCTION IF EXISTS astro;
```

then uninstall the library using command line:

```bash
sudo make uninstall
```

# Usage

## astro(date, latitude, longitude, timezone)

Returns astro info for given date, geolocation and timezone as JSON string.

### Parameter

#### date
A given valid date in 'YYYY-MM-DD hh:mm:ss' format. Invalid dates results in a NULL value.

#### latitude
North–south position of a point in degrees format

#### longitude
East-West position of a point in degrees format

#### timezone
Time zone offset from UTC in hours

### Return

The function returns the astro info as JSON string with the following keys:

| JSON Key       | Description                 | Format/Unit |
|----------------|-----------------------------|-------------|
| $.Time                        | Date for the given result | 'YYYY-MM-DDThh:mm:ss' |
| $.Zone                        | Time zone offset from UTC in hours | hour |
| $.Latitude                    | Geographic latitude | degrees |
| $.Longitude                   | Geographic longitude |degrees |
| $.deltaT                      | deltaT | decimal |
| $.JulianDate                  | Julian Date | decimal |
| $.GMST                        | Greenwich sidereal time | 'hh:mm:ss' |
| $.LMST                        | Local sidereal time | 'hh:mm:ss' |
| $.Sun.Distance.Earth          | Distance to the sun (earth's center) | km |
| $.Sun.Distance.Observer       | Distance to the sun (from the observer) | km |
| $.Sun.Ecliptic                | Ecliptic length of the sun | degrees |
| $.Sun.Declination             | Declination of the sun | degrees |
| $.Sun.Azimuth                 | Azimuth of the sun | degrees |
| $.Sun.Height                  | Height of the sun above the horizon | degrees |
| $.Sun.Diameter                | Diameter of the sun | arc seconds |
| $.Sun.Rise.Astronomical       | Astronomical dawn | 'hh:mm:ss' |
| $.Sun.Rise.Nautical           | Nautical dawn | 'hh:mm:ss' |
| $.Sun.Rise.Civil              | Civil dawn | 'hh:mm:ss' |
| $.Sun.Rise.Sunrise            | Sunrise | 'hh:mm:ss' |
| $.Sun.Culmination             | Culmination of the sun | 'hh:mm:ss' |
| $.Sun.Set.Sunset              | Sunset | 'hh:mm:ss' |
| $.Sun.Set.Civil               | Civil dusk | 'hh:mm:ss' |
| $.Sun.Set.Nautical            | Nautical dusk | 'hh:mm:ss' |
| $.Sun.Set.Astronomical        | Astronomical dusk | 'hh:mm:ss' |
| $.Sun.Ascension               | Right ascension of the sun | 'hh:mm:ss' |
| $.Sun.Zodiac                  | Zodiac | Aries, Taurus, Gemini, Cancer, Leo, Virgo, Libra, Scorpio,Sagittarius, Capricorn, Aquarius, Pisces |
| $.Moon.Distance.Earth         | Distance to the moon (earth's center) | km |
| $.Moon.Distance.Observer      | Distance to the moon (from the observer) | km |
| $.Moon.Ecliptic.Latitude      | Ecliptic latitude of the moon |degrees |
| $.Moon.Ecliptic.Longitude     | Ecliptic longitude of the moon |degrees |
| $.Moon.Declination            | Declination of the moon |degrees |
| $.Moon.Azimuth                | Azimuth of the moon |degrees |
| $.Moon.Height                 | Height of the moon above the horizon |degrees |
| $.Moon.Diameter               | Diameter of the moon | arc seconds |
| $.Moon.Rise                   | Moonrise | 'hh:mm:ss' |
| $.Moon.Culmination            | Culmination of the moon | 'hh:mm:ss' |
| $.Moon.Set                    | Moonmset | 'hh:mm:ss' |
| $.Moon.Ascension              | Right ascension of the moon | 'hh:mm:ss' |
| $.Moon.Phase.Name             | Moon phase name | New Moon, Waxing Crescent Moon, First Quarter Moon, Waxing Gibbous Moon, Full Moon, Waning Gibbous Moon, Third Quarter Moon, Waning Crescent Moon |
| $.Moon.Phase.Value            | Moon phase as index | 0..7 |
| $.Moon.Phase.Number           | Age of moon in since New Moon (0) - Full Moon (1) | decimal |
| $.Moon.Age                    | Age of moon in radians | 0..359 |
| $.Moon.Sign                   | Moon sign | Aries, Taurus, Gemini, Cancer, Leo, Virgo, Libra, Scorpio,Sagittarius, Capricorn, Aquarius, Pisces |

### Examples

Get sun rise/set

```SQL
SET @ts = NOW();
SET @latitude = 53.182153;
SET @longitude = 4.854429;
SET @timezone = TIMESTAMPDIFF(HOUR, UTC_TIMESTAMP(), NOW());

SELECT
    JSON_VALUE(astro(@ts, @latitude, @longitude, @timezone), '$.Time') AS `Time`,
    JSON_VALUE(astro(@ts, @latitude, @longitude, @timezone), '$.Sun.Rise.Sunrise') AS `Sunrise`,
    JSON_VALUE(astro(@ts, @latitude, @longitude, @timezone), '$.Sun.Set.Sunset') AS `Sunset`;
```

returns

```sql
+---------------------+----------+----------+
| Time                | Sunrise  | Sunset   |
+---------------------+----------+----------+
| 2023-01-18T09:00:00 | 08:44:23 | 16:58:02 |
+---------------------+----------+----------+
```

Get some moon info

```SQL
SET @ts = NOW();
SET @latitude = 53.182153;
SET @longitude = 4.854429;
SET @timezone = TIMESTAMPDIFF(HOUR, UTC_TIMESTAMP(), NOW());

SELECT 
    JSON_VALUE(astro(@ts, @latitude, @longitude, @timezone), '$.Time') AS `Time`,
    JSON_VALUE(astro(@ts, @latitude, @longitude, @timezone), '$.Moon.Rise') AS `Moonrise`,
    JSON_VALUE(astro(@ts, @latitude, @longitude, @timezone), '$.Moon.Set') AS `Moonset`,
    JSON_VALUE(astro(@ts, @latitude, @longitude, @timezone), '$.Moon.Phase.Name') AS `Phase`,
    ROUND(CAST(JSON_VALUE(astro(@ts, @latitude, @longitude, @timezone), '$.Moon.Age') AS FLOAT) / 3.6, 0) AS `% Age`;
```

returns

```sql
+---------------------+----------+----------+-----------------+-------+
| Time                | Moonrise | Moonset  | Phase           | % Age |
+---------------------+----------+----------+-----------------+-------+
| 2023-01-18T09:00:00 | 05:33:24 | 12:52:19 | Waning crescent |    86 |
+---------------------+----------+----------+-----------------+-------+
```

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
