const MAX_RES_LEN = 255;
typedef string NearestAirports<MAX_RES_LEN>;

struct Coordinates{
	double lat;
	double lon;
};

program AIRPORT_PROG {
        version AIRPORT_VERS {
                NearestAirports GET_FIVE_NEAREST_AIRPORTS(Coordinates) = 1;
} = 1;
} = 0x37777878;

