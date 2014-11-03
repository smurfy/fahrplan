BlackBerry World ChangeLog
==========================

2.0.20
------

- Added more translations and updated existing
- Added showing train direction for multiple backends
- Improved performance for sbb.ch, reiseinfo.no, rejseplanen.dk and 9292.nl backends
- Switched oebb.at to other backend protocol


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
