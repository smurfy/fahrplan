BlackBerry World ChangeLog
==========================

2.0.27
------

- Save trainrestrictions to settings
- Backends are now sorted
- Updated Vasttrafik's backend to version 2
- Updated and Fixed ResRobot backend
- Added Matka.fi backend

2.0.26
------

- Improvements to EFA (Dubai, Ireland, Munich, Victoria, Salzburg, SF
  Bay, and Sydney) service.
- Improvements to Hafas Binary (bahn.de, and OBB) service.
- 9292ov.nl: Display train number in journey details and timetable,
  attributes in journey details, distance to a station when searching by
  coordinates, and walking duration.
- New: Greek translation.


2.0.25
------

- Display parser short name on main page.
- Icon refresh: now it's square, like most BlackBerry 10 icons.
- New: Slovenian translation.
- Fix: Rendering of details page could get borken (occured with MVV, but
  possible with all backends).
- Fix: Text not fully visible on 720x720 resolution devices. Also, all
  context menu items should be displayed now without need to scroll.


2.0.24
------

New services:

- MVV (Germany, Munich) - supports via station and GPS location.
- SVV (Austria, Salzburg) - supports via station and GPS location.
- ResRobot (Sweden) - supports GPS location.

New features and improvements:

- Service names now follow the same pattern.
- Defunct London service was removed.
- New: Swedish translation.


2.0.23
------
- Fix: "Earlier" / "Later" was broken in bahn.de service


2.0.22
------
- Fix: bahn.de service works again
- New: Improved About screen


2.0.21
------

- New: Display of announcements for most services.
- New: Display of canceled trains / stops in journey details for bahn.de
  and öbb.at services.
- New: Display of parallel / alternative trains for bahn.de and öbb.at
  services.
- New: Display of routing type (i.e., transfer, walk, bike, car; if
  provided) for bahn.de and öbb.at services.
- New: Display of realtime train information in timetable for sbb.ch
  service.
- New: Polish translation.
- Fix: Empty entries in journey search results when using "Earlier" /
  "Later" button.
- Fix: Some stops were displayed with wrong encoding in öbb.at service.
- Fix: "Walk for %2 min" message
- Proper splash screens for BlackBerry devices with 1280x720 and
  1440x1440.


2.0.20
------

- New: Train direction is displayed in journey details for most
  services.
- New: If available, additional information about the train is displayed
  in journey details for most services.
- New: Duration of walk is shown for bahn.de.
- New: Information about train cancellation is displayed for bahn.de and
  öbb.at services.
- Performance improvements for sbb.ch, reiseinfo.no, rejseplanen.dk and
  9292ov.nl services.
- öbb.at now uses new protocol which brings performance improvements and
  uses less traffic.
- Additional info about journey is no longer added to calendar entry if
  compact format is selected in settings.
- New translations: Romanian, Persian and Dutch.
- Existing translations were updated.
- You can now easily translate Fahrplan into your own language: see link
  in the About page.


2.0.19
------

- Real-time delay info is now displayed in journey details for bahn.de.
- Bugfix: Via station was ignored in bahn.de backend.


2.0.18
------

- New backend: 9292ov.nl.
- More train selection options in rejseplanen.dk backend.


2.0.16
------

- Defunct and duplicate Sydney backend (131500.com.au) was removed.
- Translations update.


2.0.15
------

New backends:
- ptv.vic.gov.au (Victoria, Australia) - supports via station;
- Sydney (Australia) - supports via station;
- SF Bay (SanFrancisco, USA) - supports via station;
- London (London, United Kingdom) - supports via station;
- Ireland - supports via station;
- Dubai - supports via station.

New features and improvements:
- date and time is now formatted based on the phone language;
- it is now possible to switch between old and new (more compact, introduced
  in 2.0.14) formats of description in calendar events.
- few small UI improvements.


2.0.14.1
--------

Maintenance release that contains one small UI improvement: the number
of items displayed in context menus and selection dialogs is now based
on the screen height instead of hardcoded value (was 5 for portrait and
3 for landscape orientation).


2.0.14
------

A lot of work was done on the backends that fixes bugs and brings some
performance and memory usage improvements. In particular, a "Station
name is too ambiguous" error which happened with some stations is now
gone.

Additional improvements and changes:
- possibility to select target calendar to add journeys to;
- possibility to select favorite icon position;
- favorites and selected stations are now saved per backend;
- better error reporting for bahn.de backend;
- few small UI improvements.

Please, note that due to changes in the way backends work (stations are
now referenced by ID instead of the name), it's not possible to reuse
currently saved favorites. This means that after upgrading, your list of
favorites will be lost. Unfortunately, there is no way to convert old
favorites into new format.


2.0.13.2
--------

- Support for BlackBerry Q5 &amp; Q10.
- Landscape orientation support on BlackBerry Z10.

Please, note, that due to some technical grounds, after upgrading to
this version you might get two Fahrplan icons on your home screen. It's
safe to remove the old one. Alternatively, instead of upgrading you can
delete and reinstall Fahrplan. Unfortunately, in both cases your
favorites will be lost.


2.0.13
------

Update to upstream 2.0.13 version.

New features:
- Settings page.

Fixes:
- sbb.ch should work again;
- via station, if set, wasn't added to calendar event title.
