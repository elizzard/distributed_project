const MAXLEN = 255;
typedef string airportname<MAXLEN>;
typedef string nearestairports<MAXLEN>;

struct coordinates_airport{
	double lat;
	double lon;
};

program AIRPORT_PROG {
        version AIRPORT_VERS {
                nearestairports GET_FIVE_NEAREST_AIRPORTS(coordinates_airport) = 1;
} = 1;
} = 0x37777747;

