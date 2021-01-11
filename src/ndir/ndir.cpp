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
		static void usage() {
			puts("NDIR. Create directory named as current date (and time)."
				"Copyright 2000-2020 by Sergey Vasyutin.\n"
				"Usage:\n"
				"\tndir <-t>\n"
				"Switches:\n"
				"\t-t - add time to the directory name;\n"
				"\t-m - year and month only;\n"
				"\t-p - only print the name for a new directory, the directory is not being created.");
		}
	};

	// ---
	bool PrintOnly = false, AddTime = false, YearAndMonthOnly = false;
	
	for(int i = 1; i < Argc_; ++i) {
		const char* Arg = Argv_[i];
		if(!strcmp(Arg, "-h") || !strcmp(Arg, "-H") || !strcmp(Arg, "/?")) {
			THelper::usage();
			return 0;
		}
		else if(!strcmp(Arg, "-t") || !strcmp(Arg, "-T")) {
			AddTime = true;
		}
		else if(!strcmp(Arg, "-m") || !strcmp(Arg, "-M")) {
			YearAndMonthOnly = true;
		}
		else if(!strcmp(Arg, "-p") || !strcmp(Arg, "-P")) {
			PrintOnly = true;
		}
		else {
			printf("Invalid switch '%s'.\n", Arg);
			THelper::usage();
			return 1;
		}
	}

	char DirName[64];
	SYSTEMTIME st;

	GetLocalTime(&st);
	GetDateFormatA(LOCALE_SYSTEM_DEFAULT, 0, &st, YearAndMonthOnly ? "yy-MM": "yy-MM-dd", DirName, 20);
	if(AddTime) {
		GetTimeFormatA(LOCALE_SYSTEM_DEFAULT, TIME_NOSECONDS | TIME_FORCE24HOURFORMAT,
			&st, "_HH-mm", DirName + strlen(DirName), 20);
	}
	if(PrintOnly) {
		puts(DirName);
		return 0;
	}
	return (CreateDirectoryA(DirName, NULL) != 0)? 0: 1;
}