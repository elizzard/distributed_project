/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "Places.h"
#include "file_proc.h"
#include <iostream>
#include "Airports.h"
using namespace std;


coordinates* 
get_coordinates_1_svc(airportname *argp, struct svc_req *rqstp)
{
	static coordinates result ;
	string s = *argp;				
	/* strip designation*/
	stripDesignation(s);	/* shoudld do nothing*/
	/* do search*/
	Coord xy(360,360); 		/* this is where coordinate values will be returned*/
	int res = db->find(s, xy);	
	/* value of res explains the result:*/
 	/* if not found (res <0), return NULL*/ 
	/* else if found (res >=0), call airports fxn ...*/
	/*cout<< res << " "<<s<<" ["<<xy.lat << "," << xy.lon <<"]"<<endl;*/
	if(res <0)
	return NULL;

	else
	{
        result.lat = xy.lat;
        result.lon = xy.lon;
	}

	
	
	/* return-fwd KNN results*/


	return &result;
}
/*This method calls the Airports server and returns back the output*/
nearestairports *
get_nearest_airports_1_svc(coordinates *argp, struct svc_req *rqstp)
{
	CLIENT *clnt;
        static nearestairports  result_1;
        coordinates_airport  points;
	points.lat = argp->lat;
	points.lon = argp->lon;

	#ifndef DEBUG
        clnt = clnt_create ("cs2.seattleu.edu", AIRPORT_PROG, AIRPORT_VERS, "tcp");
        if (clnt == NULL) {
                clnt_pcreateerror ("cs2.seattleu.edu");
                exit (1);
        }
	#endif  /* DEBUG */

        nearestairportnames *nearestnames = get_five_nearest_airports_1(&points, clnt);
        if (nearestnames == (nearestairports *) NULL) {
                clnt_perror (clnt, "call failed");
        }
	#ifndef DEBUG
        clnt_destroy (clnt);
	#endif   /* DEBUG */

	result_1 = *nearestnames;
	return &result_1;
}
