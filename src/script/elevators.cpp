/*
	Skyscraper 1.12 Alpha - Script Processor - Elevator Section
	Copyright (C)2003-2024 Ryan Thoryk
	https://www.skyscrapersim.net
	https://sourceforge.net/projects/skyscraper/
	Contact - ryan@skyscrapersim.net

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "globals.h"
#include "sbs.h"
#include "skyscraper.h"
#include "enginecontext.h"
#include "elevator.h"
#include "wall.h"
#include "scriptproc.h"
#include "section.h"

using namespace SBS;

namespace Skyscraper {

ScriptProcessor::ElevatorSection::ElevatorSection(ScriptProcessor *parent) : Section(parent)
{

}

int ScriptProcessor::ElevatorSection::Run(std::string &LineData)
{
	//Process elevators

	//create elevator if not created already
	Simcore->NewElevator(config->Current);

	//replace variables with actual values
	ReplaceAll(LineData, "%elevator%", ToString(config->Current));

	//IF/While statement stub (continue to global commands for processing)
	if (SetCaseCopy(LineData.substr(0, 2), false) == "if" || SetCaseCopy(LineData.substr(0, 5), false) == "while")
		return sContinue;

	//process math functions
	if (MathFunctions(LineData) == sError)
		return sError;

	//process functions
	if (parent->FunctionProc() == true)
		return sNextLine;

	//get text after equal sign
	bool equals = true;
	if ((int)LineData.find("=", 0) == -1)
		equals = false;
	std::string value = GetAfterEquals(LineData);

	//get elevator object
	Elevator *elev = Simcore->GetElevator(config->Current);

	//parameters
	if (StartsWithNoCase(LineData, "name"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		elev->Name = value;
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "type"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		elev->Type = value;
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "speed"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		Real Speed;
		if (!IsNumeric(str, Speed))
			return ScriptError("Invalid value");
		elev->UpSpeed = Speed;
		elev->DownSpeed = Speed;
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "upspeed"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, elev->UpSpeed))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "downspeed"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, elev->DownSpeed))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "acceleration"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, elev->Acceleration))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "deceleration"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, elev->Deceleration))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "acceljerk"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, elev->AccelJerk))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "deceljerk"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, elev->DecelJerk))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "assignedshaft"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, elev->AssignedShaft))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "motorstartsound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		elev->MotorUpStartSound = value;
		elev->MotorDownStartSound = value;
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "motorupstartsound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		elev->MotorUpStartSound = value;
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "motordownstartsound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		elev->MotorDownStartSound = value;
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "motorrunsound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		elev->MotorUpRunSound = value;
		elev->MotorDownRunSound = value;
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "motoruprunsound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		elev->MotorUpRunSound = value;
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "motordownrunsound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		elev->MotorDownRunSound = value;
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "motorstopsound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		elev->MotorUpStopSound = value;
		elev->MotorDownStopSound = value;
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "motorupstopsound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		elev->MotorUpStopSound = value;
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "motordownstopsound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		elev->MotorDownStopSound = value;
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "motoridlesound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		elev->MotorIdleSound = value;
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "floorskiptext"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		elev->SetFloorSkipText(value);
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "recallfloor"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		int floortemp;
		std::string str = Calc(value);
		if (!IsNumeric(str, floortemp))
			return ScriptError("Invalid value");
		elev->SetRecallFloor(floortemp);
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "alternaterecallfloor"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		int floortemp;
		std::string str = Calc(value);
		if (!IsNumeric(str, floortemp))
			return ScriptError("Invalid value");
		elev->SetAlternateRecallFloor(floortemp);
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "acpfloor"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		int floortemp;
		std::string str = Calc(value);
		if (!IsNumeric(str, floortemp))
			return ScriptError("Invalid value");
		elev->SetACPFloor(floortemp);
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "motorposition"))
	{
		int params = SplitAfterEquals(LineData);
		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 2; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		elev->MotorPosition = Vector3(ToFloat(tempdata[0]), ToFloat(tempdata[1]), ToFloat(tempdata[2]));
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "queueresets"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		elev->QueueResets = ToBool(value);
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "limitqueue"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		elev->LimitQueue = ToBool(value);
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "acp"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		elev->ACP = ToBool(value);
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "uppeak"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		elev->UpPeak = ToBool(value);
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "downpeak"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		elev->DownPeak = ToBool(value);
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "inspectionservice"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		elev->InspectionService = ToBool(value);
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "fireservice1"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		int value2;
		std::string str = Calc(value);
		if (!IsNumeric(str, value2))
			return ScriptError("Invalid value");

		elev->FireServicePhase1 = value2;
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "parking"))
	{
		int params = SplitAfterEquals(LineData);
		if (params != 2)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 1; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		elev->ParkingFloor = ToInt(tempdata[0]);
		elev->ParkingDelay = ToFloat(tempdata[1]);
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "levelingspeed"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		Real leveling;
		std::string str = Calc(value);
		if (!IsNumeric(str, leveling))
			return ScriptError("Invalid value");
		elev->LevelingSpeed = leveling;
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "levelingoffset"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		Real leveling;
		std::string str = Calc(value);
		if (!IsNumeric(str, leveling))
			return ScriptError("Invalid value");
		elev->LevelingOffset = leveling;
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "levelingopen"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		Real leveling;
		std::string str = Calc(value);
		if (!IsNumeric(str, leveling))
			return ScriptError("Invalid value");
		elev->LevelingOpen = leveling;
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "notifyearly"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		Real notify;
		std::string str = Calc(value);
		if (!IsNumeric(str, notify))
			return ScriptError("Invalid value");
		elev->NotifyEarly = notify;
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "departuredelay"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		Real delay;
		std::string str = Calc(value);
		if (!IsNumeric(str, delay))
			return ScriptError("Invalid value");
		elev->DepartureDelay = delay;
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "arrivaldelay"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		Real delay;
		std::string str = Calc(value);
		if (!IsNumeric(str, delay))
			return ScriptError("Invalid value");
		elev->ArrivalDelay = delay;
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "inspectionspeed"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, elev->InspectionSpeed))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "autodoors"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		elev->AutoDoors = ToBool(value);
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "openonstart"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		elev->OpenOnStart = ToBool(value);
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "interlocks"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		elev->Interlocks = ToBool(value);
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "floorhold"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		elev->FloorHold = ToBool(value);
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "motoremergencystopsound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		elev->MotorEmergencyStopSound = value;
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "emergencystopspeed"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, elev->EmergencyStopSpeed))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "chimeonarrival"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		elev->ChimeOnArrival = ToBool(value);
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "reopen"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		elev->ReOpen = ToBool(value);
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "hoistwayaccesshold"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		elev->HoistwayAccessHold = ToBool(value);
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "runstate"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		elev->SetRunState(ToBool(value));
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "ropeposition"))
	{
		int params = SplitAfterEquals(LineData);
		if (params != 3)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 0; i <= 2; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		elev->RopePosition = Vector3(ToFloat(tempdata[0]), ToFloat(tempdata[1]), ToFloat(tempdata[2]));
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "ropetexture"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		elev->RopeTexture = value;
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "counterweightstartsound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		elev->CounterweightStartSound = value;
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "counterweightmovesound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		elev->CounterweightMoveSound = value;
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "counterweightstopsound"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		//check to see if file exists
		parent->CheckFile("data/" + value);

		elev->CounterweightStopSound = value;
		return sNextLine;
	}
	if (StartsWithNoCase(LineData, "id "))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		elev->ID = Calc(value);
		return sNextLine;
	}

	//CreateElevator command
	if (StartsWithNoCase(LineData, "createelevator"))
	{
		//get data
		int params = SplitData(LineData, 15);

		if (params != 4)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 1; i <= 3; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		bool result = elev->CreateElevator(ToBool(tempdata[0]), ToFloat(tempdata[1]), ToFloat(tempdata[2]), ToInt(tempdata[3]));
		if (result == false)
			return ScriptError();
		StoreCommand(elev);
		return sNextLine;
	}

	//CreateCounterweight command
	if (StartsWithNoCase(LineData, "createcounterweight"))
	{
		//get data
		int params = SplitData(LineData, 20);

		if (params != 8)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 7; i++)
		{
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		Vector3 size = Vector3(ToFloat(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]));
		StoreCommand(elev->CreateCounterweight(tempdata[0], tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]), size, ToFloat(tempdata[7])));
		return sNextLine;
	}

	//AddRails command
	if (StartsWithNoCase(LineData, "addrails"))
	{
		//get data
		int params = SplitData(LineData, 9);

		if (params != 7)
			return ScriptError("Incorrect number of parameters");

		//check numeric values
		for (int i = 2; i <= 6; i++)
		{
			if (i == 4)
				i++;
			if (!IsNumeric(tempdata[i]))
				return ScriptError("Invalid value: " + tempdata[i]);
		}

		elev->AddRails(tempdata[0], tempdata[1], ToFloat(tempdata[2]), ToFloat(tempdata[3]), ToBool(tempdata[4]), ToFloat(tempdata[5]), ToFloat(tempdata[6]));
		return sNextLine;
	}

	//process elevator car commands for default car
	int result = parent->GetElevatorCarSection()->Run(LineData);
	if (result != sContinue)
		return result;

	//handle end of elevator section
	if (StartsWithNoCase(LineData, "<endelevator>") && config->RangeL == config->RangeH)
	{
		config->SectionNum = 0;
		config->Context = "None";
		engine->Report("Finished elevator");
		return sNextLine;
	}

	//handle elevator range
	if (config->RangeL != config->RangeH && StartsWithNoCase(LineData, "<endelevator"))
	{
		if (config->Current < config->RangeH)
		{
			config->Current++;
			parent->line = config->RangeStart;  //loop back
			return sNextLine;
		}
		else
		{
			config->SectionNum = 0; //break out of loop
			config->Context = "None";
			config->RangeL = 0;
			config->RangeH = 0;
			engine->Report("Finished elevators");
			return sNextLine;
		}
	}

	return sContinue;
}

}
