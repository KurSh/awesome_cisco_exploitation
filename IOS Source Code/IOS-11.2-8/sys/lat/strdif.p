/*-----------------------------------------------------------------------
    str_diff		(i) compare counted text strings

    Copyright 1989,1990	Meridian Technology Corporation, St. Louis, MO

    This material is confidential information of Meridian Technology
    Corporation.  Furnished under license.  All rights reserved.
 -----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------
    This function is used by the LAT engine to compare two counted text
    strings. The fuction returns zero if strings are equal, positive if
    dst is alphabetically greater than src, and negative otherwise.

    CALL BY:	int str_diff(dst,src)
    		byte	*dst;
    		byte	*src;

    ON EXIT:	Return zero if strings are equal, positive if dst is
    		alphabetically greater than src, and negative otherwise.
 -----------------------------------------------------------------------*/
#include "mtcsys.h"
#include "latfnc.h"

extern	byte	c_codes[256];		/* Character code table		*/

int str_diff(dst,src)

    byte	*dst;			/* Pointer to string descriptor	*/
    byte	*src;			/* Pointer to string descriptor	*/
{
    int		smallest;		/* Length of shortest string	*/
    int		temp;			/* Comparision results		*/
    int         val;                    /* Return argument		*/

/*-----------------------------------------------------------------------
    Determine which string has the smallest length, and preset val based
    on the results.
 -----------------------------------------------------------------------*/
    if (src == 0)
    {
	smallest = 0;
	val = 1;
    }
    else
    {
	if ((val = *dst - *src) > 0)
	    smallest = *src;
        else
    	    smallest = *dst;
    }

/*-----------------------------------------------------------------------
    Compare characters for alphabetic precedence.  If no precedence is
    found assume the strings are equal.
 -----------------------------------------------------------------------*/
    for (src++, dst++; smallest > 0; --smallest)
    {
#if LE_UPPER
	if ((temp = *dst++ - *src++) != 0)
    	{
    	    val = temp;
    	    break;
    	}
#else
	{
	    byte  c1, c2;

	    c1 = *dst++;
	    c2 = *src++;

	    if (c_codes[c1] == 2)
	        c1 -= 32;

	    if (c_codes[c2] == 2)
	        c2 -= 32;

	    if ((temp = c1 - c2) != 0)
    	    {
    	        val = temp;
    	        break;
	    }
    	}
#endif
    }
    return(val);
}
