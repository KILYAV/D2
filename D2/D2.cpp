#define STORE_LOAD_IMPLEMENTATION
#include "../../utility/store_load.h"

#include "uniform.h"
#include "frame.h"
#include "shade.h"
#include "race.h"
//#include "battle.h"

const Frame::Instant Frame::frame{ 800, 600 };

const Shade::Resource Shade::resource;
const Shade::Race Shade::race;

const Race::Resource Race::resource;

int main(int argc, char** argv)
{
	Race first{ Race::GetRace() };
}
