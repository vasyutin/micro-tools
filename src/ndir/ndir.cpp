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
enum class TMode {Date, DateTime, MonthYear, Help, Error};


//---------------------------------------------------------------------------
int main(int Argc_, char *Argv_[])
{
	struct THelper {
		static void usage() {
			printf("NDIR. Create directory named as current date (and time)."
				"Copyright 2000-2020 by Sergey Vasyutin.\n"
				"Usage:\n"
				"\tndir <-t>\n"
				"Switches:\n"
				"\t-t - add time to directory name;\n"
				"\t-m - year and month.");
		}
	};

	// ---
	TMode g_Mode = TMode::Date;

	if(Argc_ == 2) {
		const char* Arg = Argv_[1];
		if(!strcmp(Arg, "-h") || !strcmp(Arg, "-H") || !strcmp(Arg, "/?")) {
			g_Mode = TMode::Help;
		}
		else if(!strcmp(Arg, "-t") || !strcmp(Arg, "-T")) {
			g_Mode = TMode::DateTime;
		}
		else if(!strcmp(Arg, "-m") || !strcmp(Arg, "-M")) {
			g_Mode = TMode::MonthYear;
		}
		else {
			g_Mode = TMode::Error;
		}
	}

	if(TMode::Error == g_Mode) {
		printf("Invalid switch '%s'.\n", Argv_[1]);
		THelper::usage();
		return 1;
	}
	if(TMode::Help == g_Mode) {
		THelper::usage();
		return 0;
	}

	char DirName[64];
	SYSTEMTIME st;

	GetLocalTime(&st);
	GetDateFormatA(LOCALE_SYSTEM_DEFAULT, 0, &st, TMode::MonthYear == g_Mode? "yy-MM": "yy-MM-dd", DirName, 20);
	if(TMode::DateTime == g_Mode) {
		GetTimeFormatA(LOCALE_SYSTEM_DEFAULT, TIME_NOSECONDS | TIME_FORCE24HOURFORMAT,
			&st, "_HH-mm", DirName + strlen(DirName), 20);
	}
	return (CreateDirectoryA(DirName, NULL) != 0)? 0: 1;
}