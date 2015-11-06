/**************************************************************************************************
	Module:       Utility.cpp
	Author:       Laurent Noel
	Date created: 04/08/05

	General utility functions and definitions - only included when needed

	Copyright 2005-2006, University of Central Lancashire and Laurent Noel

	Change history:
		V1.0    Created 04/08/05 - LN
**************************************************************************************************/


#include "Utility.h"

namespace gen
{

/*------------------------------------------------------------------------------------------------
	String utilities
 ------------------------------------------------------------------------------------------------*/

// Return first substring in a string list separated by a given delimiter
	std::string FirstDelimitedSubstr
(
	const std::string& sList,
	const std::string& sDelimiter
)
{
	std::string::size_type firstDelimiter = sList.find_first_of( sDelimiter );
	if (firstDelimiter == std::string::npos)
	{
		return sList;
	}
	else
	{
		return sList.substr( firstDelimiter - sDelimiter.length() + 1 );
	}
}

// Return last substring in a string list separated by a given delimiter
	std::string LastDelimitedSubstr
(
	const std::string& sList,
	const std::string& sDelimiter
)
{
	std::string::size_type lastDelimiter = sList.find_last_of( sDelimiter );
	if (lastDelimiter == std::string::npos)
	{
		return sList;
	}
	else
	{
		return sList.substr( lastDelimiter + 1 );
	}
}


} // namespace gen
