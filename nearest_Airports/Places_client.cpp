/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "Places.h"
#include<iostream>
#include<sstream>

using namespace std;

void
places_prog_1(char *name,char *host)
{
	CLIENT *clnt;
	coordinates  *result_1;
	airportname  airportName;
	nearestairports  *result_2;
	coordinates  placeCoordinates;

#ifndef	DEBUG
	clnt = clnt_create (host, PLACES_PROG, PLACES_VERS, "tcp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	airportName = name;
	cout<<"Airport Name is"<<airportName<<endl;
	result_1 = get_coordinates_1(&airportName, clnt);
	if (result_1 == (coordinates *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	/*else
	{
		placeCoordinates = result_1;
	}*/
	result_2 = get_nearest_airports_1(result_1, clnt);
	if (result_2 == (nearestairports *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	cout<<"%%%%%%%"<<result_2;
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;
	
	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	/*Converting char to string*/
	
	char *city = argv[2];
	char *state = argv[3];
	int size = strlen(city) + strlen(state) +2;
	char *name = (char *)malloc(size);
	strcpy(name,city);
	strcat(name," ");
	strcat(name,state);
	cout<<"Place name is "<<name<<endl;
	places_prog_1 (name,host);
exit (0);
}
