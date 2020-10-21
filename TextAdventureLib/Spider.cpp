#include "Spider.h"
#include "Enums.h"

#include <iostream>

void Spider::printEntryDescription()
{
	std::cout << "A " << Enums::printSize(m_size) << " spider crawls from a crack in the ceiling and takes up an attack posture!\n";
}