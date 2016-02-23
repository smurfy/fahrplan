//          Copyright Erik Lundin 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <boost/program_options.hpp>

#include "gausskruger.h"

using namespace gausskruger;
namespace po = boost::program_options;

#define EXE_NAME "gausskruger"

class ParameterProjection : public Projection
{
public:
    ParameterProjection(double flattening, double equatorialRadius, double centralMeridian,
            double scale, double falseNorthing, double falseEasting) :
        mFlattening(flattening),
        mEquatorialRadius(equatorialRadius),
        mCentralMeridian(centralMeridian),
        mScale(scale),
        mFalseNorthing(falseNorthing),
        mFalseEasting(falseEasting) {}
    double flattening() { return mFlattening; }
    double equatorialRadius() { return mEquatorialRadius; }
    double centralMeridian() { return mCentralMeridian; }
    double scale() { return mScale; }
    double falseNorthing() { return mFalseNorthing; }
    double falseEasting() { return mFalseEasting; }
private:
    double mFlattening;
    double mEquatorialRadius;
    double mCentralMeridian;
    double mScale;
    double mFalseNorthing;
    double mFalseEasting;
};

int main(int argc, char *argv[])
{
    double inverseFlattening;
    double equatorialRadius;
    double centralMeridian;
    double scale;
    double falseNorthing;
    double falseEasting;
    std::vector<double> coords;
    int nDecimals;

    try {
        // Projection parameters
        po::options_description projection_parameters("Projection parameters (mandatory)");
        projection_parameters.add_options()
                ("invflattening,i", po::value<double>(&inverseFlattening),
                        "inverse flattening of the ellipsoid")
                ("radius,a", po::value<double>(&equatorialRadius),
                        "equatorial radius, a.k.a. semi-major axis of the ellipsoid")
                ("meridian,m", po::value<double>(&centralMeridian),
                        "longitude of the central meridian")
                ("scale,s", po::value<double>(&scale),
                        "scale factor along the central meridian")
                ("falsenorthing,n", po::value<double>(&falseNorthing),
                        "false northing")
                ("falseeasting,e", po::value<double>(&falseEasting),
                        "false easting")
                ;

        // Options
        po::options_description options("Options");
        options.add_options()
                ("help,h", "print this help")
                ("decimals,d", po::value<int>(&nDecimals)->default_value(3), "number of decimals")
                ("reverse,r",
                        "reverse transformation (grid to geodetic), default is geodetic to grid")
                ;

        // Positional parameters (input coordinates)
        po::options_description hidden_parameters;
        hidden_parameters.add_options()
                ("coords", po::value<std::vector<double> >(&coords))
                ;
        po::positional_options_description positional_parameters;
        positional_parameters.add("coords", 2);

        // Store the parameters
        po::options_description all_parameters;
        all_parameters.add(options).add(projection_parameters).add(hidden_parameters);
        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(all_parameters).positional(positional_parameters).run(), vm);
        po::notify(vm);

        // Print help (if requested or no other options is given)
        po::options_description visible_parameters;
        visible_parameters.add(projection_parameters).add(options);
        if (argc == 1 || vm.count("help")) {
            std::cout << "Usage: " << EXE_NAME << " <projection> [options] latitude longitude\n"
                      << "       " << EXE_NAME << " <projection> [options] -r northing easting\n"
                      << visible_parameters << std::endl;
            return 1;
        }

        // Check for all mandatory parameters
        if (!(vm.count("invflattening") && vm.count("radius") && vm.count("meridian") && vm.count("scale")
                && vm.count("falsenorthing") && vm.count("falseeasting"))) {
            std::cout << "Missing mandatory projection parameter(s)" << std::endl;
            return 2;
        }
        if (coords.size() != 2) {
            std::cerr << "Exactly two coordinate values have to be entered" << std::endl;
            return 3;
        }

        // Set number of decimals in output
        std::cout.setf(std::ios::fixed);
        std::cout.precision(nDecimals);

        // Do the actual transformation
        ParameterProjection projection(1 / inverseFlattening, equatorialRadius,
                centralMeridian, scale, falseNorthing, falseEasting);
        if (vm.count("reverse")) {
            double lat, lon;
            projection.gridToGeodetic(coords.at(0), coords.at(1), lat, lon);
            std::cout << std::fixed << "Latitude: " << lat << "\nLongitude: " << lon << std::endl;
        } else {
            double northing, easting;
            projection.geodeticToGrid(coords.at(0), coords.at(1), northing, easting);
            std::cout << std::fixed << "Northing: " << northing << "\nEasting: " << easting << std::endl;
        }
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 4;
    }

    return 0;
}
