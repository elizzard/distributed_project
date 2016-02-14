const MAXLEN = 255;
typedef string airportname<MAXLEN>;
typedef string nearestairports<MAXLEN>;

struct coordinates{
double lat;
double lon;
};

program PLACES_PROG {
        version PLACES_VERS {
                coordinates  GET_COORDINATES(airportname) = 1;
		nearestairports GET_NEAREST_AIRPORTS(coordinates)=2;

} = 1;
} = 0x37777735;

