---
layout: default
title: Index
---

Fahrplan
========

A journey planner and timetable application for a lot of train and
public transport lines in Europe, USA and Australia.

Supported Operating Systems/Devices:
------------------------------------
* Meego Harmattan: Nokia N9 and N950.
* Nokia's Symbian touch phones (Symbian^3, Anna and Belle).
* BlackBerry 10[^1].
* Ubuntu Touch.
* Sailfish OS.

Fahrplan Features:
------------------

* Plan a journey from one station/stop to another (with one via station
  if supported by provider).
* Select type of transportation (depends on provider).
* Real-time timetable information for a station/stop (with direction if
  supported by provider).
* Find a nearby station based on your GPS location (if supported by
  provider).
* Add station to favorites for quick access in future.
* Add journey to calendar.

Download and install for your device:
-------------------------------------

* [BlackBerry 10][2]
* [N9, N950][5]
* [Symbian][3]
* [Ubuntu Touch][4]
* Sailfish OS - search in the store for "fahrplan"

Currently supported Providers:
------------------------------

* bahn.de (Germany and Europe) - supports via station and GPS location.
* sbb.ch (Switzerland) - support via station, timetable with direction and GPS location.
* rejseplanen.dk (Denmark) - supports via station and GPS location.
* oebb.at (Austria) - supports via station and GPS location.
* reiseinfo.no (Norway) - supports GPS location.
* vasttrafik.se (western Sweden) - supports via station and GPS location.
* ptv.vic.gov.au (Victoria, Australia) - supports via station.
* Sydney (NSW, Sydney, Australia) - supports via station.
* SF Bay (SanFrancisco, USA) - supports via station.
* London (London, United Kingdom) - supports via station.
* Ireland - supports via station.
* Dubai - supports via station.
* 9292ov.nl (Netherlands) - supports via station and GPS location.

Compiling:
----------

Please consult the [compiling documentation][1]

Source:
-------

Source code for all versions is available at <http://github.com/smurfy/fahrplan>.

For old no longer maintained code of fahrplan (used on the N900 only) visit: <https://garage.maemo.org/plugins/scmsvn/viewcvs.php/?root=fahrplan>

Copyright:
----------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program.  If not, see <http://www.gnu.org/licenses/>.

---

[^1]: It's not possible to input umlauts and accents on hardware
      keyboard models currently, due to some technical limitations.
      This issue will be resolved as soon as [#100][] is implemented.

[1]: compile
[2]: https://appworld.blackberry.com/webstore/content/28277436/?lang=en&countrycode=DE
[3]: download
[4]: https://launchpad.net/~mzanetti/+archive/ppa
[5]: https://openrepos.net/content/smurfy/fahrplan
[#100]: https://github.com/smurfy/fahrplan/issues/100
