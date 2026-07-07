#pragma once

/* Cheat name */
#define _PRODUCT_NAME              "EBLENIX"

/* Window class name for CS */
#define CSGO_CLASS_NAME            "Valve001"

/* Main directory of the cheat */
#define BASE_DIRECTORY_NAME        "EBX/"

/* Path to the configurations directory */
#define CFG_DIRECTORY_PATHS        BASE_DIRECTORY_NAME "cfg/"

/* Path to the configuration startup file */
#define CFG_STARTUP_FILE_PATH      BASE_DIRECTORY_NAME "cfg_startup.dat"

/* Configuration keys */
#define CFG_INT_KEY                "{i32}"
#define CFG_FLOAT_KEY              "{f32}"
#define CFG_BOOL_TRUE_KEY          "{on}"
#define CFG_BOOL_FALSE_KEY         "{off}"

/* Path to the lua scripts directory */
#define LUA_DIRECTORY_PATHS        BASE_DIRECTORY_NAME "lua/"

/* Path to the lua startup list file */
#define LUA_STARTUP_LIST_PATH      BASE_DIRECTORY_NAME "lua_startup.json"

/* Enable/disable lua scripts, if undefined, scripts are disabled */
#define LUA_ENABLED

enum fn_num {
	// Hook indexes in virtual tables

	CREATE_MOVE_FN_INDEX = 24,
	PAINT_TRAVERSE_FN_INDEX = 41,
	PRESENT_FN_INDEX = 17,
	RESET_FN_INDEX = 16,
	DO_POST_SCREEN_EFFECTS_FN_INDEX = 44,
	SCENE_END_FN_INDEX = 9,
	SCREEN_VIEWMODEL_FOV_CHANGE_FN_INDEX = 35,
	GET_SCREEN_ASPECT_RATIO_FN_INDEX = 101,
	DRAW_MODEL_EXECUTE_FN_INDEX = 21,
	OVERRIDE_VIEW_FN_INDEX = 18,
	IS_CONNECTED_FN_INDEX = 27,
	SV_CHEATS_BOOLEAN_FN_INDEX = 13,
	DRAW_SET_COLOR_FN_INDEX = 15,
	FRAME_STAGE_NOTIFY_FN_INDEX = 37,
	LEVEL_INIT_POST_ENTITY_FN_INDEX = 6,
	SHUTDOWN_FN_INDEX = 4,
};