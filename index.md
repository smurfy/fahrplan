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

* Germany (bahn.de) - supports via station and GPS location.
* Germany, Munich (mvv-muenchen.de)
* Austria (oebb.at) - supports via station and GPS location.
* Austria, Salzburg (svv-info.at)* Denmark (rejseplanen.dk) - supports via station and GPS location.
* Switzerland (sbb.ch) - support via station, timetable with direction and GPS location.
* Norway (reiseinfo.no / nri) - supports GPS location.
* Netherlands / Belgium (9292ov.nl) - supports via station and GPS location.
* Sweden (vasttrafik.se) - supports via station and GPS location.
* Sweden (resrobot.se)
* Ireland (transportforireland.ie) - supports via station.
* Australia, Victoria (ptv.vic.gov.au) - supports via station.
* Australia, Sydney (transportnsw.info) - supports via station.
* USA, SF Bay (511.org) - supports via station.
* Dubai (rta.ae) - supports via station.

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
[3]: http://symbian.apps.opera.com/fahrplan.html
[4]: https://appstore.bhdouglass.com/app/com.ubuntu.developer.mzanetti.fahrplan2
[5]: https://openrepos.net/content/smurfy/fahrplan
[#100]: https://github.com/smurfy/fahrplan/issues/100
