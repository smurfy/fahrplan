Simple Gauss-Krüger transformation for C++
==========================================
This project provides a C++ implementation of coordinate transformation between geodetic
coordinates and grid coordinates of Gauss (Transverse Mercator) projections, using the Krüger-n series developed to fourth order.

A lot of inspiration comes from the `Java implementation <https://github.com/kodapan/gauss-kruger>`_ made by Kodapan, primarily in the way classes are used. That project is in turn a port of the `Javascript implementation <http://latlong.mellifica.se/>`_ made by Arnold Andreasson. The equations and numbers used are directly based on the `formulas published by Lantmäteriet <http://www.lantmateriet.se/sv/Kartor-och-geografisk-information/GPS-och-geodetisk-matning/Om-geodesi/Formelsamling/>`_ (the Swedish National Land Survey), especially the document called *Gauss Conformal Projection (Transverse Mercator) - Krüger's Formulas*, published in 2008, and tests are implemented to check the calculations against reference points provided by Lantmäteriet.

This C++ implementation was originally developed for coordinate conversion in the public transport
application *Fahrplan* since using Proj4 seemed overly complex, and the intent is to make it simple to
include in other projects wishing to use it. Although it was developed in a Swedish context, with Swedish coordinate systems to test with, it is thought to be useful also for other coordinate systems using the Gauss-Krüger projection.

Except the library, a simple command line tool is included so you can play around with the transformations.

Dependencies
------------
The library itself requires no more than standard library components. To build the test, *Boost Test* is required, and for the command line tool, *Boost Program Options* is required (tested using version 1.54 of Boost).

Usage
-----
The library can be compiled and linked dynamically or statically, or included in your project as source code files. In the former cases, it has to be compiled first::

    $ make lib

Code
^^^^
The values in the following example are taken from *Parametrar till approximativa
transformationssamband för direktprojektion mellan SWEREF 99 och RT 90*, published by Lantmäteriet (projection: 7.5 gon V 0:-15). If you have more than one projections using the same ellipsoid, the ellipsoid values can be put in a class of their own::

    #include <iostream>
    #include "gausskruger.h"

    class GRS80 : public gausskruger::Projection
    {
    public:
        double flattening() { return 1 / 298.257222101; }
        double equatorialRadius() { return 6378137.0; }
    };

    class RT90_75_gon_V : public GRS80
    {
    public:
        double centralMeridian() { return 11 + 18.375 / 60.0; }
        double scale() { return 1.000006; }
        double falseNorthing() { return -667.282; }
        double falseEasting() { return 1500025.141; }
    };

    int main(int argc, char* argv[])
    {
        RT90_75_gon_V projection;
        double lat, lon, northing, easting;
        lat = 67 + 52.654 / 60;
        lon = 21 + 3.615 / 60;
        projection.geodeticToGrid(lat, lon, northing, easting);
        std::cout.setf(std::ios::fixed);
        std::cout.precision(3);
        std::cout << "Northing: " << northing << "\nEasting: " << easting << std::endl;
        return 0;
    }

Command line tool
^^^^^^^^^^^^^^^^^
First build the tool::

    $ make cli

Example of direct transformation from SWEREF 99 to RT90 (same values as from the library usage example)::

    $ ./gausskruger --invflattening 298.257222101 --radius 6378137 --meridian 11.30625 \
                    --scale 1.000006 --falsenorthing -667.282 --falseeasting 1500025.141 \
                    67.877566667 21.06025
    Northing: 7563929.530
    Easting: 1908686.715

To see all options, run the tool without options or with ``--help``::

    $ ./gausskruger --help
    Usage: gausskruger <projection> [options] latitude longitude
           gausskruger <projection> [options] -r northing easting

    Projection parameters (mandatory):
      -i [ --invflattening ] arg inverse flattening of the ellipsoid
      -a [ --radius ] arg        equatorial radius, a.k.a. semi-major axis of the
                                 ellipsoid
      -m [ --meridian ] arg      longitude of the central meridian
      -s [ --scale ] arg         scale factor along the central meridian
      -n [ --falsenorthing ] arg false northing
      -e [ --falseeasting ] arg  false easting

    Options:
      -h [ --help ]              print this help
      -d [ --decimals ] arg (=3) number of decimals
      -r [ --reverse ]           reverse transformation (grid to geodetic), default
                                 is geodetic to grid


Tests
^^^^^
To verify the calculations, tests have been implemented to check the reference points provided by Lantmäteriet. The following documents have been used:

* *Kontrollpunkter för SWEREF 99 TM*
* *Kontrollpunkter för de lokala SWEREF 99-zonerna*
* *Parametrar till approximativa transformationssamband för direktprojektion mellan SWEREF 99 och RT 90*
* *Kontrollpunkter RT90 ↔ SWEREF 99*

To run the tests, execute the following::

    $ make test
    $ ./test_reference_points
    Running 101 test cases...

    *** No errors detected

License
-------
gauss-kruger-cpp is licensed under the Boost Software License 1.0.
