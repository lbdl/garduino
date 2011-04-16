/*
*	File: ui.h
*
*	a set of strings to use with 
*	so we can send a basic ui over serial
*	as a simple gui
*
*/
#include "myio.h"

#ifndef _UI_H
#define _UI_H


#define LN "**********"
#define NL "\n\r"
#define TAB "\t"



void main_menu(void);
void main_menu2(void);
void data_menu(void);
void overide_menu(void);
void overide_msg(void);
void temp_disp(void);
void moist_disp(void);
void light_disp(void);

#endif
