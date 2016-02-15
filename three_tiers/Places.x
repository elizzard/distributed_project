const MAXLEN = 255;
typedef string location_name<MAXLEN>;
typedef string nearestairports<MAXLEN>;


program PLACES_PROG {
        version PLACES_VERS {
                nearestairports  GET_COORDINATES(location_name) = 1;
} = 1;
} = 0x37778778;

