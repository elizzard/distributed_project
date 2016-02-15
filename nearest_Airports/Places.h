/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _PLACES_H_RPCGEN
#define _PLACES_H_RPCGEN

#include <rpc/rpc.h>
#ifdef __cplusplus
extern "C" {
#endif

#define MAXLEN 255


typedef char *airportname;

typedef char *nearestairports;

struct coordinates {
	double lat;
	double lon;
};
typedef struct coordinates coordinates;
#define PLACES_PROG 0x37777735
#define PLACES_VERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define GET_COORDINATES 1
extern  coordinates * get_coordinates_1(airportname *, CLIENT *);
extern  coordinates * get_coordinates_1_svc(airportname *, struct svc_req *);
#define GET_NEAREST_AIRPORTS 2
extern  nearestairports * get_nearest_airports_1(coordinates *, CLIENT *);
extern  nearestairports * get_nearest_airports_1_svc(coordinates *, struct svc_req *);
extern int places_prog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define GET_COORDINATES 1
extern  coordinates * get_coordinates_1();
extern  coordinates * get_coordinates_1_svc();
#define GET_NEAREST_AIRPORTS 2
extern  nearestairports * get_nearest_airports_1();
extern  nearestairports * get_nearest_airports_1_svc();
extern int places_prog_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_airportname (XDR *, airportname*);
extern  bool_t xdr_nearestairports (XDR *, nearestairports*);
extern  bool_t xdr_coordinates (XDR *, coordinates*);

#else /* K&R C */
extern bool_t xdr_airportname ();
extern bool_t xdr_nearestairports ();
extern bool_t xdr_coordinates ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_PLACES_H_RPCGEN */
