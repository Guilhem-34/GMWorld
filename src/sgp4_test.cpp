#include <iostream>
#include <libsgp4/Tle.h>
#include <libsgp4/SGP4.h>
#include <libsgp4/Eci.h>
#include <libsgp4/CoordGeodetic.h>
#include <libsgp4/DateTime.h>
#include <libsgp4/Util.h>

int main()
{
    // TLE de l'ISS récupéré sur Celestrak
    libsgp4::Tle tle(
        "ISS (ZARYA)",
        "1 25544U 98067A   26220.50489838  .00004539  00000+0  89319-4 0  9991",
        "2 25544  51.6323  41.1734 0007358  25.6275 334.5077 15.49385107579869"
        );

    // Le "propagateur" : l'objet qui sait calculer une position à partir du TLE
    libsgp4::SGP4 sgp4(tle);

    // Heure actuelle (UTC)
    libsgp4::DateTime maintenant = libsgp4::DateTime::Now();

    // Calcule la position à cet instant (coordonnées ECI, un repère centré sur la Terre)
    libsgp4::Eci position = sgp4.FindPosition(maintenant);

    // Convertit en coordonnées "humaines" : latitude, longitude, altitude
    libsgp4::CoordGeodetic geo = position.ToGeodetic();

    std::cout << "Position de l'ISS :" << std::endl;
    std::cout << "  Latitude  : " << libsgp4::Util::RadiansToDegrees(geo.latitude) << " deg" << std::endl;
    std::cout << "  Longitude : " << libsgp4::Util::RadiansToDegrees(geo.longitude) << " deg" << std::endl;
    std::cout << "  Altitude  : " << geo.altitude << " km" << std::endl;

    return 0;
}