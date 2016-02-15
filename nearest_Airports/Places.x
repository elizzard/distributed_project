const MAXLEN = 512;
typedef string airportname<MAXLEN>;
typedef string nearestairports<MAXLEN>;
typedef string host<MAXLEN>;

struct places_input{
airportname apname;
host hostname;
};
union airports_ret switch (int err) {
case 0:
nearestairports airports; /*no error, return airports*/
case 1:
void; /*error occured, nothing returned*/
};

program PLACES_PROG {
        version PLACES_VERS {
                airports_ret GET_COORDINATES(places_input) = 1;

} = 1;
} = 0x37777735;

