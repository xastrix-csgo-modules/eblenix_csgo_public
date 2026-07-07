#pragma once

#include <string>
#include <vector>
#include <thread>

#include "common.h"

enum _module_list {
	clientDLL,
	engineDLL,
	shaderapidx9DLL,
	vstdlibDLL,
	vgui2DLL,
	vguiMatSurfaceDLL,
	materialsystemDLL,
	serverBrowserDLL,
	tier0DLL,
	localizeDLL,
	maxModules,
};

enum _state {
	S_INIT_BASE,
	S_INIT_VARS,
	S_INIT_HOOKS,
	S_WAITING_FOR_SHUTDOWN,
	S_SHUTDOWN,
};

enum _boolean_flags {
	BF_INITIALISED,
	BF_PANIC,
	BF_CONSOLE_OPENED,
	BF_CHAT_OPENED,
	BF_NIGHTMODE_HOLD,
	BF_BOMB_PLANTED,
	maxBooleanFlags,
};

enum _integer_flags {
	IF_SECONDS_IN_GAME,
	IF_MINUTES_IN_GAME,
	IF_HOURS_IN_GAME,
	IF_BOMB_TIMER,
	IF_BOMB_SITE_ID,
	maxIntegerFlags,
};

namespace g
{
	inline bool b_flags[maxBooleanFlags]{};
	inline int i_flags[maxIntegerFlags]{};

	inline int state;
	inline std::string module_list[maxModules] = {
		"client.dll",
		"engine.dll",
		"shaderapidx9.dll",
		"vstdlib.dll",
		"vgui2.dll",
		"vguimatsurface.dll",
		"materialsystem.dll",
		"serverbrowser.dll",
		"tier0.dll",
		"localize.dll",
	};

	void handle_playing_time(const std::chrono::steady_clock::time_point start_time, int interval);
	void unload();
}

#define GLOBAL(x) g::x