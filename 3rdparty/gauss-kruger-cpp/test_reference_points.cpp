//          Copyright Erik Lundin 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_DYN_LINK test_reference_points
#include <boost/test/included/unit_test.hpp>
#include <boost/bind.hpp>

#include "gausskruger.h"

using gausskruger::Projection;
using namespace boost::unit_test;

class GRS80 : public Projection
{
public:
    double flattening() { return 1 / 298.257222101; }
    double equatorialRadius() { return 6378137.0; }
};

class SWEREF99TM : public GRS80
{
public:
    double centralMeridian() { return 15.0; }
    double scale() { return 0.9996; }
    double falseNorthing() { return 0.0; }
    double falseEasting() { return 500000.0; }
};

class LocalSWEREF99 : public GRS80
{
public:
    LocalSWEREF99(double centralMeridian) :
            mCentralMeridian(centralMeridian) {}
    double centralMeridian() { return mCentralMeridian; }
    double scale() { return 1.0; }
    double falseNorthing() { return 0.0; }
    double falseEasting() { return 150000.0; }
private:
    double mCentralMeridian;
};

class RT90_GRS80 : public GRS80
{
public:
    RT90_GRS80(double centralMeridian, double scale, double falseN, double falseE) :
            mCentralMeridian(centralMeridian), mScale(scale), mFalseN(falseN), mFalseE(falseE) {}
    double centralMeridian() { return mCentralMeridian; }
    double scale() { return mScale; }
    double falseNorthing() { return mFalseN; }
    double falseEasting() { return mFalseE; }
private:
    double mCentralMeridian;
    double mScale;
    double mFalseN;
    double mFalseE;
};

void testSWEREF99TM(double lat, double lon, double n, double e)
{
    static const double gridTolerance = 0.001;
    static const double degreeTolerance = 1.0 / 60;

    SWEREF99TM projection;
    double latOut, lonOut, nOut, eOut;
    projection.geodeticToGrid(lat, lon, nOut, eOut);
    BOOST_CHECK_SMALL(nOut - n, gridTolerance);
    BOOST_CHECK_SMALL(eOut - e, gridTolerance);
    projection.gridToGeodetic(n, e, latOut, lonOut);
    BOOST_CHECK_SMALL(latOut - lat, degreeTolerance);
    BOOST_CHECK_SMALL(lonOut - lon, degreeTolerance);
}

void testLocalSWEREF99(double centralMeridian, double lat, double lon, double n, double e)
{
    static const double gridTolerance = 0.001;
    static const double degreeTolerance = 1.0 / 60;

    LocalSWEREF99 projection(centralMeridian);
    double latOut, lonOut, nOut, eOut;
    projection.geodeticToGrid(lat, lon, nOut, eOut);
    BOOST_CHECK_SMALL(nOut - n, gridTolerance);
    BOOST_CHECK_SMALL(eOut - e, gridTolerance);
    projection.gridToGeodetic(n, e, latOut, lonOut);
    BOOST_CHECK_SMALL(latOut - lat, degreeTolerance);
    BOOST_CHECK_SMALL(lonOut - lon, degreeTolerance);
}

void testDirectProjRT90_SWEREF99(double centralMeridian, double scale, double falseN, double falseE,
        double lat, double lon, double n, double e)
{
    static const double gridTolerance = 0.001;
    static const double degreeTolerance = 0.001 / 60;

    RT90_GRS80 projection(centralMeridian, scale, falseN, falseE);
    double latOut, lonOut, nOut, eOut;
    projection.geodeticToGrid(lat, lon, nOut, eOut);
    BOOST_CHECK_SMALL(nOut - n, gridTolerance);
    BOOST_CHECK_SMALL(eOut - e, gridTolerance);
    projection.gridToGeodetic(n, e, latOut, lonOut);
    BOOST_CHECK_SMALL(latOut - lat, degreeTolerance);
    BOOST_CHECK_SMALL(lonOut - lon, degreeTolerance);
}

/**
 * This is the RT 90 projection for all of Sweden
 * Values from the section "Transformation RT 90 - SWEREF 99" on Lantmäteriet's website
 */
void testDirectProjRT90_25gonV_SWEREF99(double lat, double lon, double n, double e)
{
    testDirectProjRT90_SWEREF99(15 + 48.0 / 60 + 22.624306 / 3600, 1.00000561024,
            -667.711, 1500064.274,
            lat, lon, n, e);
}

/**
 * Tests direct projection between RT 90 and SWEREF 99 TM (grid)
 */
void testDirectProjRT90_25gonV_SWEREF99TM(double nRT90, double eRT90, double nSWEREF99TM, double eSWEREF99TM)
{
    static const double gridTolerance = 0.001;

    RT90_GRS80 projectionRT90(15 + 48.0 / 60 + 22.624306 / 3600, 1.00000561024,
            -667.711, 1500064.274);
    SWEREF99TM projectionSWEREF99TM;
    double lat, lon, nOut, eOut;

    // RT90 -> SWEREF (lat,lon) -> SWEREF99TM
    projectionRT90.gridToGeodetic(nRT90, eRT90, lat, lon);
    projectionSWEREF99TM.geodeticToGrid(lat, lon, nOut, eOut);
    BOOST_CHECK_SMALL(nOut - nSWEREF99TM, gridTolerance);
    BOOST_CHECK_SMALL(eOut - eSWEREF99TM, gridTolerance);

    // SWEREF99TM -> SWEREF (lat,lon) -> RT90
    projectionSWEREF99TM.gridToGeodetic(nSWEREF99TM, eSWEREF99TM, lat, lon);
    projectionRT90.geodeticToGrid(lat, lon, nOut, eOut);
    BOOST_CHECK_SMALL(nOut - nRT90, gridTolerance);
    BOOST_CHECK_SMALL(eOut - eRT90, gridTolerance);
}

/**
 * Local RT 90 projections:
 * Values from "Parametrar till approximativa transformationssamband för direktprojektion
 * mellan SWEREF 99 och RT 90"
 */

void testDirectProjRT90_75gonV_SWEREF99(double lat, double lon, double n, double e)
{
    testDirectProjRT90_SWEREF99(11 + 18.375 / 60, 1.000006, -667.282, 1500025.141, lat, lon, n, e);
}

void testDirectProjRT90_5gonV_SWEREF99(double lat, double lon, double n, double e)
{
    testDirectProjRT90_SWEREF99(13 + 33.376 / 60, 1.0000058, -667.130, 1500044.695, lat, lon, n, e);
}

void testDirectProjRT90_0gon0_SWEREF99(double lat, double lon, double n, double e)
{
    testDirectProjRT90_SWEREF99(18 + 3.378 / 60, 1.0000054, -668.844, 1500083.521, lat, lon, n, e);
}

void testDirectProjRT90_25gonO_SWEREF99(double lat, double lon, double n, double e)
{
    testDirectProjRT90_SWEREF99(20 + 18.379 / 60, 1.0000052, -670.706, 1500102.765, lat, lon, n, e);
}

void testDirectProjRT90_5gonO_SWEREF99(double lat, double lon, double n, double e)
{
    testDirectProjRT90_SWEREF99(22 + 33.38 / 60, 1.0000049, -672.557, 1500121.846, lat, lon, n, e);
}

bool initFunction()
{
    // Reference points from the document "Kontrollpunkter för SWEREF 99 TM",
    // published by Lantmäteriet 2007-11-20

    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testSWEREF99TM, 55.0, 12.75, 6097106.672, 356083.438)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testSWEREF99TM, 55.0, 14.25, 6095048.642, 452024.069)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testSWEREF99TM, 57.0, 12.75, 6319636.937, 363331.554)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testSWEREF99TM, 57.0, 19.5, 6326392.707, 773251.054)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testSWEREF99TM, 59.0, 11.25, 6546096.724, 284626.066)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testSWEREF99TM, 59.0, 19.5, 6548757.206, 758410.519)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testSWEREF99TM, 61.0, 12.75, 6764877.311, 378323.440)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testSWEREF99TM, 61.0, 18.75, 6768593.345, 702745.127)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testSWEREF99TM, 63.0, 12.0, 6989134.048, 348083.148)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testSWEREF99TM, 63.0, 19.5, 6993565.630, 727798.671)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testSWEREF99TM, 65.0, 13.5, 7209293.753, 429270.201)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testSWEREF99TM, 65.0, 21.75, 7225449.115, 817833.405)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testSWEREF99TM, 67.0, 16.5, 7432168.174, 565398.458)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testSWEREF99TM, 67.0, 24.0, 7459745.672, 891298.142)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testSWEREF99TM, 69.0, 21.0, 7666089.698, 739639.195)));

    //-------------------------------------------------------------------------

    // Reference points from the document "Kontrollpunkter för de lokala SWEREF 99-zonerna",
    // published by Lantmäteriet 2007-11-20

    // SWEREF 99 <-> SWEREF 99 12 00
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 12.0, 57.0, 11.25, 6320164.077, 104421.390)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 12.0, 59.0, 11.25, 6542910.921, 106894.103)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 12.0, 57.0, 12.75, 6320164.077, 195578.610)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 12.0, 59.0, 12.75, 6542910.921, 193105.897)));

    // SWEREF 99 <-> SWEREF 99 13 30
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 13.5, 55.0, 12.75, 6097487.637, 102004.871)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 13.5, 57.0, 12.75, 6320164.077, 104421.390)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 13.5, 59.0, 12.75, 6542910.921, 106894.103)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 13.5, 61.0, 12.75, 6765725.847, 109420.005)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 13.5, 63.0, 12.75, 6988606.198, 111996.020)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 13.5, 55.0, 14.25, 6097487.637, 197995.129)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 13.5, 57.0, 14.25, 6320164.077, 195578.610)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 13.5, 59.0, 14.25, 6542910.921, 193105.897)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 13.5, 61.0, 14.25, 6765725.847, 190579.995)));

    // SWEREF 99 <-> SWEREF 99 14 15
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 14.25, 63.0, 12.0, 6990379.288, 36003.367)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 14.25, 65.0, 13.5, 7211548.993, 114619.001)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 14.25, 63.0, 15.0, 6988606.198, 188003.980)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 14.25, 65.0, 15.0, 7211548.993, 185380.999)));

    // SWEREF 99 <-> SWEREF 99 15 00
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 15.0, 55.0, 14.25, 6097487.637, 102004.871)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 15.0, 57.0, 14.25, 6320164.077, 104421.390)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 15.0, 59.0, 14.25, 6542910.921, 106894.103)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 15.0, 61.0, 14.25, 6765725.847, 109420.005)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 15.0, 57.0, 15.75, 6320164.077, 195578.610)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 15.0, 59.0, 15.75, 6542910.921, 193105.897)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 15.0, 61.0, 15.75, 6765725.847, 190579.995)));

    // SWEREF 99 <-> SWEREF 99 15 45
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 15.75, 63.0, 15.0, 6988606.198, 111996.020)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 15.75, 65.0, 15.0, 7211548.993, 114619.001)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 15.75, 67.0, 15.0, 7434550.943, 117285.739)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 15.75, 63.0, 16.5, 6988606.198, 188003.980)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 15.75, 65.0, 16.5, 7211548.993, 185380.999)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 15.75, 67.0, 16.5, 7434550.943, 182714.261)));

    // SWEREF 99 <-> SWEREF 99 16 30
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 16.5, 57.0, 15.75, 6320164.077, 104421.390)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 16.5, 59.0, 15.75, 6542910.921, 106894.103)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 16.5, 61.0, 15.75, 6765725.847, 109420.005)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 16.5, 63.0, 15.75, 6988606.198, 111996.020)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 16.5, 57.0, 17.25, 6320164.077, 195578.610)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 16.5, 59.0, 17.25, 6542910.921, 193105.897)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 16.5, 61.0, 17.25, 6765725.847, 190579.995)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 16.5, 63.0, 17.25, 6988606.198, 188003.980)));

    // SWEREF 99 <-> SWEREF 99 17 15
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 17.25, 63.0, 16.5, 6988606.198, 111996.020)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 17.25, 65.0, 16.5, 7211548.993, 114619.001)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 17.25, 67.0, 16.5, 7434550.943, 117285.739)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 17.25, 63.0, 18.0, 6988606.198, 188003.980)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 17.25, 65.0, 18.0, 7211548.993, 185380.999)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 17.25, 67.0, 18.0, 7434550.943, 182714.261)));

    // SWEREF 99 <-> SWEREF 99 18 00
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 18.0, 59.0, 17.25, 6542910.921, 106894.103)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 18.0, 61.0, 17.25, 6765725.847, 109420.005)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 18.0, 59.0, 18.75, 6542910.921, 193105.897)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 18.0, 61.0, 18.75, 6765725.847, 190579.995)));

    // SWEREF 99 <-> SWEREF 99 18 45
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 18.75, 57.0, 18.0, 6320164.077, 104421.390)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 18.75, 59.0, 18.0, 6542910.921, 106894.103)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 18.75, 63.0, 18.0, 6988606.198, 111996.020)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 18.75, 65.0, 18.0, 7211548.993, 114619.001)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 18.75, 67.0, 18.0, 7434550.943, 117285.739)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 18.75, 57.0, 19.5, 6320164.077, 195578.610)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 18.75, 59.0, 19.5, 6542910.921, 193105.897)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 18.75, 63.0, 19.5, 6988606.198, 188003.980)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 18.75, 65.0, 19.5, 7211548.993, 185380.999)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 18.75, 67.0, 19.5, 7434550.943, 182714.261)));

    // SWEREF 99 <-> SWEREF 99 20 15
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 20.25, 63.0, 19.5, 6988606.198, 111996.020)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 20.25, 65.0, 19.5, 7211548.993, 114619.001)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 20.25, 67.0, 19.5, 7434550.943, 117285.739)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 20.25, 69.0, 19.5, 7657608.465, 119992.964)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 20.25, 65.0, 21.0, 7211548.993, 185380.999)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 20.25, 67.0, 21.0, 7434550.943, 182714.261)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 20.25, 69.0, 21.0, 7657608.465, 180007.036)));

    // SWEREF 99 <-> SWEREF 99 21 45
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 21.75, 65.0, 21.0, 7211548.993, 114619.001)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 21.75, 67.0, 21.0, 7434550.943, 117285.739)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 21.75, 65.0, 22.5, 7211548.993, 185380.999)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 21.75, 67.0, 22.5, 7434550.943, 182714.261)));

    // SWEREF 99 <-> SWEREF 99 23 15
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 23.25, 65.0, 22.5, 7211548.993, 114619.001)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 23.25, 67.0, 22.5, 7434550.943, 117285.739)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 23.25, 65.0, 24.0, 7211548.993, 185380.999)));
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testLocalSWEREF99, 23.25, 67.0, 24.0, 7434550.943, 182714.261)));

    //-------------------------------------------------------------------------

    // Reference points from the document
    // "Parametrar till approximativa transformationssamband för
    // direktprojektion mellan SWEREF 99 och RT 90",
    // published by Lantmäteriet 2006-08-28

    // SWEREF 99 <-> RT 90 7.5 gon V 0:-15
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testDirectProjRT90_75gonV_SWEREF99,
            67 + 52.654 / 60,
            21 + 3.615 / 60,
            7563929.530, 1908686.715)));

    // SWEREF 99 <-> RT 90 5 gon V 0:-15
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testDirectProjRT90_5gonV_SWEREF99,
            67 + 52.654 / 60,
            21 + 3.615 / 60,
            7550738.132, 1814885.068)));

    // SWEREF 99 <-> RT 90 0 gon 0:-15
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testDirectProjRT90_0gon0_SWEREF99,
            67 + 52.654 / 60,
            21 + 3.615 / 60,
            7534665.780, 1626336.062)));

    // SWEREF 99 <-> RT 90 2.5 gon O 0:-15
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testDirectProjRT90_25gonO_SWEREF99,
            67 + 52.654 / 60,
            21 + 3.615 / 60,
            7531788.718, 1531799.452)));

    // SWEREF 99 <-> RT 90 5 gon O 0:-15
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testDirectProjRT90_5gonO_SWEREF99,
            67 + 52.654 / 60,
            21 + 3.615 / 60,
            7532352.152, 1437227.702)));

    //-------------------------------------------------------------------------

    // Reference points from the document
    // "Kontrollpunkter RT90 ↔ SWEREF 99",
    // published by Lantmäteriet 2006-05-03

    // A: SWEREF 99 <-> RT 90 (direct projection)
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testDirectProjRT90_25gonV_SWEREF99,
            67 + 5.0 / 60 + 26.452769 / 3600,
            21 + 2.0 / 60 + 5.101575 / 3600,
            7453389.762, 1727060.905)));

    // A: RT 90 <-> SWEREF 99 TM (direct projection)
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testDirectProjRT90_25gonV_SWEREF99TM,
            7453389.762, 1727060.905,
            7454204.638, 761811.242)));

    // B: SWEREF 99 <-> RT 90 (direct projection)
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testDirectProjRT90_25gonV_SWEREF99,
            63 + 32.0 / 60 + 14.761735 / 3600,
            16 + 14.0 / 60 + 59.594626 / 3600,
            7047738.415, 1522128.637)));

    // B: RT 90 <-> SWEREF 99 TM (direct projection)
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testDirectProjRT90_25gonV_SWEREF99TM,
            7047738.415, 1522128.637,
            7046077.605, 562140.337)));

    // C: SWEREF 99 <-> RT 90 (direct projection)
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testDirectProjRT90_25gonV_SWEREF99,
            60 + 9.0 / 60 + 33.882413 / 3600,
            14 + 45.0 / 60 + 28.167152 / 3600,
            6671665.273, 1441843.186)));

    // C: RT 90 <-> SWEREF 99 TM (direct projection)
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testDirectProjRT90_25gonV_SWEREF99TM,
            6671665.273, 1441843.186,
            6669189.376, 486557.055)));

    // D: SWEREF 99 <-> RT 90 (direct projection)
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testDirectProjRT90_25gonV_SWEREF99,
            56 + 21.0 / 60 + 17.199245 / 3600,
            13 + 52.0 / 60 + 23.754022 / 3600,
            6249111.351, 1380573.079)));

    // D: RT 90 <-> SWEREF 99 TM (direct projection)
    framework::master_test_suite().add(BOOST_TEST_CASE(boost::bind(
            &testDirectProjRT90_25gonV_SWEREF99TM,
            6249111.351, 1380573.079,
            6246136.458, 430374.835)));

    return true;
}

int main(int argc, char* argv[])
{
    return unit_test_main(&initFunction, argc, argv);
}
