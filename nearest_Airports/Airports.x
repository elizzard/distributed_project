const MAXLEN = 512;
typedef string nearestairportnames<MAXLEN>;

struct coordinates_airport{
	double lat;
	double lon;
};
union airports_server_ret switch (int err) {
case 0:
nearestairportnames airports; /*no error, return airports*/
case 1:
void; /*error occured, nothing returned*/
};
program AIRPORT_PROG {
        version AIRPORT_VERS {
                nearestairportnames GET_FIVE_NEAREST_AIRPORTS(coordinates_airport) = 1;
} = 1;
} = 0x37777747;

