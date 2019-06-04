//---------------------------------------------------------------------------
// NDir
//
// Creates directory with the name which is constructed of current date (and time).
//
// Written by Sergey Vasyutin (sergey [at] vasyut.in).
//---------------------------------------------------------------------------
#include <windows.h>
#include <stdio.h>

//---------------------------------------------------------------------------
int main(int Argc_, char *Argv_[])
{
struct THelper {
	static bool askingHelp(const char *Arg_) {
		if(!strcmp(Arg_, "-h") || 
			!strcmp(Arg_, "-H") ||
			!strcmp(Arg_, "/?"))
			return true;
		return false;
		}

	// ---
	static bool addTime(const char *Arg_) {
		if(!strcmp(Arg_, "-t") || !strcmp(Arg_, "-T"))
			return true;
		return false;
		}
	};

if(Argc_ == 2 && THelper::askingHelp(Argv_[1])) {
	printf("NDIR. Create directory named as current date (and time)."
		"Copyright 2000-2019 by Sergey Vasyutin.\n"
		"Usage:\n"
		"\tndir <-t>\n"
		"Switches:\n"
		"\t-t - add time to directory name.");
	return 0;
	}

char DirName[50];
SYSTEMTIME st;

GetLocalTime(&st);
GetDateFormatA(LOCALE_SYSTEM_DEFAULT, 0, &st, "yyMMdd", DirName, 20);

if(Argc_ == 2 && THelper::addTime(Argv_[1])) {
   GetTimeFormatA(LOCALE_SYSTEM_DEFAULT, TIME_NOSECONDS | TIME_FORCE24HOURFORMAT,
		&st, "-HHmm", DirName + 6, 20);
   }
return (CreateDirectoryA(DirName, NULL) != 0)? 0: 1;
}