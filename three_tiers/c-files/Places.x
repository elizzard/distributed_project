const MAX_RES_LEN = 255;
const MAX_NAME_LEN = 60;
const STATE_LEN = 2;
typedef string LocationName<MAX_NAME_LEN>;
typedef string StateName<STATE_LEN>;
typedef string NearestAirports<MAX_RES_LEN>;

struct LocationArgs{
	LocationName name;
	StateName state;
};

program PLACES_PROG {
    version PLACES_VERS {
        NearestAirports  GET_AIRPORTS(LocationArgs) = 1;
} = 1;
} = 0x37777778;

