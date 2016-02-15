const MAXLEN = 255;
typedef string airportname<MAXLEN>;
typedef string nearestairports<MAXLEN>;


program PLACES_PROG {
        version PLACES_VERS {
                nearestairports  GET_COORDINATES(airportname) = 1;
} = 1;
} = 0x37777735;

