#include "util.h"

int main(int argc, char* argv[])
{
	std::string process_name = (argc > 1) ? argv[1] : "csgo.exe";
	std::string dll_name = (argc > 2) ? argv[2] : "eblenix_csgo.dll";

	DWORD pid = util::get_proc_id(process_name.c_str());

	if (!pid) {
		printf("[-] game is not running... (%s > %s)\n", dll_name.c_str(), process_name.c_str());
		return 1;
	}

	return util::inject(pid, dll_name);
}