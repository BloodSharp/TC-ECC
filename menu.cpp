#include "menu.h"
#include "cvar.h"
#include "gui.h"
#include "color.h"
#include "drawing.h"
#include "TableFont.h"

#pragma warning(disable:4018)
#pragma warning(disable:4800)
#pragma warning(disable:4244)
#pragma warning(disable:4101)
#pragma warning(disable:4715)
#pragma warning(disable:4305)

cMenu gMenu;

void cMenu::Draw(int x, int y)
{
	if(!Activated) return;
	int i, w, h=0;
	w = 100;
	for(int i=0; i < Entrys.size(); i++) h+=11; // Font Size
	gGui.window(x, y, w, h, 0.5, "Rumpelstielzchen");
	for(i=0; i < Entrys.size(); i++)
	{
		ColorEntry * clr;
		if(i == iSelected)
			clr = colorList.get(8);
		else
			clr = colorList.get(7);
		PrintWithFont(x, y+i*11,clr->r,clr->g,clr->b,Entrys[i].cCVarName);
		if(Entrys[i].bIsInt)
			PrintWithFont(x+w-9,y+i*11,255,255,255,"%i",Entrys[i].iValue);
		else if(!Entrys[i].bIsInt)
			PrintWithFont(x+w-25,y+i*11,255,255,255,"%2.2f",Entrys[i].fValue[0]);
	}
}

int cMenu::Key(int keynum)
{
	if(!Activated) return 1;
	if( keynum == 128 || keynum == 240) //uparrow || mwheelup
	{
		if( iSelected>0 ) iSelected--;
		else iSelected = Entrys.size()-1;
		return 0;
	}
	else if( keynum == 129 || keynum == 239) //downarrow || mwheeldown
	{
		if( iSelected<Entrys.size()-1 ) iSelected++;
		else iSelected = 0;
		return 0;
	}
	else if( keynum == 130 || keynum == 241 ) //leftarrow || leftbutton
	{
		if( Entrys[iSelected].fValue && !Entrys[iSelected].bIsInt)
		{
			Entrys[iSelected].fValue[0] -= Entrys[iSelected].fStep;
			if( Entrys[iSelected].fValue[0] < Entrys[iSelected].fMin )
				Entrys[iSelected].fValue[0] = Entrys[iSelected].fMax;
		}
		if(Entrys[iSelected].bIsInt)
		{
			Entrys[iSelected].iValue -= Entrys[iSelected].iStep;
			if( Entrys[iSelected].iValue < Entrys[iSelected].iMin )
				Entrys[iSelected].iValue = Entrys[iSelected].iMax;
		}
		return 0;
	}
	else if( keynum == 131 || keynum == 242 ) //rightarrow || rightbutton
	{
		if( Entrys[iSelected].fValue && !Entrys[iSelected].bIsInt)
		{
			Entrys[iSelected].fValue[0] += Entrys[iSelected].fStep;
			if( Entrys[iSelected].fValue[0] > Entrys[iSelected].fMax )
				Entrys[iSelected].fValue[0] = Entrys[iSelected].fMin;
		}
		if(Entrys[iSelected].bIsInt)
		{
			Entrys[iSelected].iValue += Entrys[iSelected].iStep;
			if( Entrys[iSelected].iValue > Entrys[iSelected].iMax )
				Entrys[iSelected].iValue = Entrys[iSelected].iMin;
		}
		return 0;
	}
	return 1;
}

void cMenu::Init()
{
	#define REG_FLOAT(name,value,step,max,min) AddFloat(##step, ##max, ##&value, #name, ##min);
	REG_FLOAT(Nospread, cvar.nospread, 1, 2, 0);
	REG_FLOAT(Aimingmethod, cvar.aimingmethod, 1, 2, 1);
	REG_FLOAT(Aimspot, cvar.aimspot, 1, 3, 1);
	REG_FLOAT(Aimkey, cvar.aimkey,1,3,0);
	REG_FLOAT(Avdraw, cvar.avdraw, 1, 1, 0);
	REG_FLOAT(FOV, cvar.fov, 1, 4, 1);
	REG_FLOAT(Autowall, cvar.autowall, 1, 1, 0);
	REG_FLOAT(Recoil, cvar.recoil, 0.25, 3.0, 0.0);
	REG_FLOAT(Weapon, cvar.weapon, 1, 2, 0);
	REG_FLOAT(Name, cvar.name, 1, 1, 0);
	REG_FLOAT(Distance, cvar.distance, 1, 1, 0);
	REG_FLOAT(EntEsp, cvar.entesp, 1, 1, 0);
	REG_FLOAT(Box, cvar.box, 1, 2, 0);
	REG_FLOAT(Radar, cvar.radar, 1.0, 2.0, 0.0);
	REG_FLOAT(Wallhack, cvar.wall, 1, 2, 0);
	REG_FLOAT(Infobox, cvar.infobox, 1, 1, 0);
	REG_FLOAT(Aspeed, cvar.aspeed, 0.1, 3, 0);
	REG_FLOAT(Speed, cvar.speed, 0.1, 3, 0);
	REG_FLOAT(Knivespeed, cvar.knivespeed, 1, 1, 0);
	REG_FLOAT(Speedkey, cvar.speedkey, 1, 3, 0);
	REG_FLOAT(Crosshair, cvar.crosshair, 1, 5, 0);
	REG_FLOAT(Smoke/Flash, cvar.nosmash, 1, 1, 0);
	REG_FLOAT(Offset h, cvar.hadj_h, 0.25, 3, 0);
	REG_FLOAT(Offset f, cvar.hadj_f, 0.25, 3, 0);
	REG_FLOAT(Offset r, cvar.hadj_r, 0.25, 3, 0);
	REG_FLOAT(Autoaim, cvar.aim, 1, 1, 0);
	REG_FLOAT(Autoshoot, cvar.shoot, 1, 1, 0);
	REG_FLOAT(Spinhack, cvar.spinhack, 1, 1, 0);
	REG_FLOAT(Playerlight, cvar.dlights, 1, 1, 0);
	REG_FLOAT(Glowshells, cvar.glowshells, 1, 1, 0);
	REG_FLOAT(Extra-Blood, cvar.blood, 1, 1, 0);
	REG_FLOAT(Bunnyhop, cvar.bunnyhop, 1, 1, 0);
	REG_FLOAT(Chasecam, cvar.chasecam, 1, 2, 0);
	REG_FLOAT(Fullbright, cvar.fullbright, 1, 1, 0);
	REG_FLOAT(Whitewalls, cvar.whitewalls, 1, 1, 0);
}

void cMenu::AddInt(int step, int max, int value, char* name, int min)
{
	MenuEntry tmp;
	tmp.iStep = step;
	tmp.iMax = max;
	tmp.iValue = value;
	tmp.iMin = min;
	tmp.cCVarName = name;
	tmp.bIsInt = true;
	Entrys.push_back(tmp);
}

void cMenu::AddFloat(float step, float max, float *value, char* name, float min)
{
	MenuEntry tmp;
	tmp.fStep = step;
	tmp.fMax = max;
	tmp.fValue = value;
	tmp.fMin = min;
	tmp.cCVarName = name;
	tmp.bIsInt = false;
	Entrys.push_back(tmp);
}
