/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "Places.h"

bool_t
xdr_airportname (XDR *xdrs, airportname *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, objp, MAXLEN))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_nearestairports (XDR *xdrs, nearestairports *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, objp, MAXLEN))
		 return FALSE;
	return TRUE;
}
