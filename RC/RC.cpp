#include "domain.h"

int main()
{
	Store("../D2/resource/race", LoadRace("race"));
	Store("../D2/resource/shade", LoadShade("shade"));
	Store("../D2/resource/interf", LoadInterf("interf"));
	Store("../D2/resource/battle", LoadBattle(std::filesystem::current_path() / "battle"));
	Store("../D2/resource/bat_units", LoadBatUnits(std::filesystem::current_path() / "bat_units"));
}
