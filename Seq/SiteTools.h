/*
 * File SiteTools.h
 * Author : Julien Dutheil <julien.dutheil@ens-lyon.fr>
 * Author : Guillaume Deuchst <GDeuchst@ifrance.com>
 * Last modification : Friday August 8 2003
*/

#ifndef _SITETOOLS_H_
#define _SITETOOLS_H_

#include "SymbolListTools.h"
#include "Site.h"

// From Utils:
#include <Utils/Exceptions.h>

// From the STL:
#include <map>

using namespace std;

/* SiteTools static class
 * Implements methods to get informations about sites
 *
 * This class can't be instanciated
*/

class SiteTools: public SymbolListTools
{
	public:
		~SiteTools() {}
	
	public:
		// Method for know if a site contains gap(s) or not
		static bool hasGap(const Site & site);
	
		// Method for know if a site contains undefined character or not
		static bool hasUnknown(const Site & site);
	
		// Method fot know if a site is complete or not (doesn't contains gap or undefined character)
		static bool isComplete(const Site & site);

 		/* This method is quite the same as operator ==,
 		 * excepted that it just look at the content of the site
 		 * whatever the position.
 		 */
		static bool areSitesIdentical(const Site & site1, const Site & site2);
	
		// Method for know if a site is constant or not (contain one character type only)
		static bool isConstant(const Site & site) throw (EmptySiteException);

		static double variabilityShannon(const Site & site) throw (EmptySiteException);

		static double variabilityFactorial(const Site & site) throw (EmptySiteException);
};

#endif	//_SITETOOLS_H_