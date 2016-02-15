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

struct coordinates
{
	double lat;
	double lon;
};
nearestairports* 
get_coordinates_1_svc(airportname *argp, struct svc_req *rqstp)
{
	cout<<"In Places_Server"<<endl;
	//static coordinates result ;
	static nearestairports airportnames;
	string s = *argp;
	cout<<"Airport name in Places_server"<<s<<endl;				
	/* strip designation*/
	stripDesignation(s);	/* shoudld do nothing*/
	cout<<"After strip designations"<<endl;
	/* do search*/
	Coord xy(360,360); 		/* this is where coordinate values will be returned*/
	//int res = db->find(s, xy);	
	/* value of res explains the result:*/
 	/* if not found (res <0), return NULL*/ 
	/* else if found (res >=0), call airports fxn ...*/
	/*cout<< res << " "<<s<<" ["<<xy.lat << "," << xy.lon <<"]"<<endl;*/
	/*if(res <0)
	return NULL;

	else
	{
	cout<<"In Else Places_server"<<endl;
        result.lat = xy.lat;
        result.lon = xy.lon;
	cout<<"lat"<<result.lat<<"lon"<<result.lon<<endl;
	}*/

	
	
	/* return-fwd KNN results*/
	 CLIENT *clnt;
         coordinates_airport  points;
	 nearestairportnames *nearestnames;
        /*points.lat = result.lat;
        points.lon = result.lon;*/
	points.lat = 33.58;
        points.lon = -85.85;;

        #ifndef DEBUG
        clnt = clnt_create ("cs2.seattleu.edu", AIRPORT_PROG, AIRPORT_VERS, "tcp");
        if (clnt == NULL) {
                clnt_pcreateerror ("cs2.seattleu.edu");
                exit (1);
        }
        #endif  /* DEBUG */
        cout<<"In Places server airport code"<<"lat"<<points.lat<<"lon"<<points.lon<<endl;
        nearestnames = get_five_nearest_airports_1(&points, clnt);
	 if (nearestnames == (nearestairportnames *) NULL) {
                clnt_perror (clnt, "call failed");
        }
        #ifndef DEBUG
        clnt_destroy (clnt);
        #endif   /* DEBUG */
	cout<<"After function call to airports server"<<endl;
        cout<<"GOT THE OUTPUT"<<nearestnames<<endl;

        airportnames = *nearestnames;
        cout<<"LAAAAAAAAAAAA"<<airportnames;
        return &airportnames;

}

