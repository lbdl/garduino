#include "ui.h"
//#include "myio.h"

char* title = "Sensor Reader V1.0";

char* welc1 = "******************Sensor Reader V1.0******************\n\r";
char* welc2 = "******************T E Storey**************************\n\r";
char* caret = ">\t";
char* menuln1 = "Please choose from data, or override";
char* menuln2 = "by entering d or o";
char* tmp = "Temperature: ";
char* lgt = "Light: ";
char* mst = "Moisture: ";
char* overide = "Pump active";
char* d_m_ln1 = "Please select, temperature, light or moisture";
char* d_m_ln2 = "by entering, t, l or m";
char* o_m_ln1 = "Press o to start pump";
char* o_m_ln2 = "Pump will run for 5 minutes";
char* o_m_ln3 = "Press any key to return to menu";
char* light_ln1 = "0 = dark, 1 = twilight, 2 = cloudy, 3 = bright, 4 = sunny";
char* moist_ln1 = "0 = very dry, 1 = dry, 2 = damp, 3 = wet";

void main_menu(void)
{
	sendStr(welc1);
	sendStr(welc2);
	sendStr(NL);
	sendStr(NL);
}
//
void main_menu2(void)
{
	main_menu();
	sendStr(menuln1);
	sendStr(NL);
	sendStr(menuln2);
	sendStr(NL);
	//sendStr(caret);
}
//
//
void data_menu(void)
{
	main_menu();
	sendStr(d_m_ln1);
	sendStr(NL);
	sendStr(d_m_ln2);
	sendStr(NL);
	//sendStr(caret);
}
//
void overide_menu(void)
{
	main_menu();
	sendStr(o_m_ln1);
	sendStr(NL);
	//sendStr(caret);
}
//
void overide_msg(void)
{
	main_menu();
	sendStr(o_m_ln2);
	sendStr(NL);
	sendStr(o_m_ln3);
	sendStr(NL);
	//sendStr(caret);
}
//
void temp_disp(void)
{
	main_menu();
	sendStr(tmp);
	sendStr(NL);
	//sendStr(tmp);
}
//
void light_disp(void)
{
	main_menu();
	sendStr(light_ln1);
	sendStr(NL);
	sendStr(lgt);
}
//
void moist_disp(void)
{
	main_menu();
	sendStr(moist_ln1);
	sendStr(NL);
	sendStr(mst);
}



