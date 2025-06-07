// Copyright jdknight

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#include <windows.h>

// buffer to track registry keys to process
//
// This buffer holds an initial value of "Control Panel\NotifyIconSettings\",
// the base point for our registry monitoring. Once we setup monitoring in
// this key, we will then use the buffer to help populate registry keys that
// we will use to open and monitor/update promotion flags. An example of a
// found key may populate the value to something such as:
//
//     Control Panel\NotifyIconSettings\0123456789012345678
//
// Unknown if the identifier for each instance will be 19-characters long.
// We will be flexible in just use a buffer of 128.
#define BASE_KEY L"Control Panel\\NotifyIconSettings\\"
#define BASE_KEY_LEN ((sizeof(BASE_KEY) / sizeof(WCHAR)) - 1)
WCHAR BUFFER[128] = BASE_KEY;

// invoked when we want to check if a sysicon is promoted and update if its not
static void check_and_update_sysicon(HKEY* iconKey)
{
	// check that the sysicon is configured to be promoted
	DWORD data = 0;
	DWORD dataLen = sizeof(data);
	LONG result = RegGetValueW(
		*iconKey,
		NULL,
		L"IsPromoted",
		RRF_RT_REG_DWORD,
		NULL,
		&data,
		&dataLen
	);

	// if the key is found and is promoted, no update is required
	if (result == ERROR_SUCCESS && data == 1) {
		return;
	}

	// we want to promote this sysicon; add/update the promote value
	data = 1;
	RegSetValueExW(
		*iconKey,
		L"IsPromoted",
		0,
		REG_DWORD,
		(const BYTE*)&data,
		sizeof(data)
	);
}

// cycle through all sysicons and trigger a check/update for promotion
static void promote_all_sysicons(HKEY* baseKey)
{
	for(DWORD idx = 0;; idx++) {
		DWORD buffSpace = sizeof(BUFFER) / sizeof(BUFFER[0]) - BASE_KEY_LEN;
		LONG result = RegEnumKeyEx(
			*baseKey,
			idx,
			BUFFER + BASE_KEY_LEN,
			&buffSpace,
			NULL,
			NULL,
			NULL,
			NULL
		);

		// if we have no more sysicons to process, stop
		if (result == ERROR_NO_MORE_ITEMS) {
			break;
		// we have found a sysicon to process; open its key for reading/writing
		} else if (result == ERROR_SUCCESS) {
			HKEY iconKey;
			LONG subResult = RegOpenKeyExW(
				HKEY_CURRENT_USER,
				BUFFER,
				0,
				KEY_READ | KEY_WRITE,
				&iconKey
			);
			if (subResult == ERROR_SUCCESS) {
				// request to check/update the promotion of the sysicon
				check_and_update_sysicon(&iconKey);
				RegCloseKey(iconKey);
			}
		// for all other errors, we will stop
		} else {
			break;
		}
	}
}

#pragma warning(disable: 4100)
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
        PWSTR pCmdLine, int nCmdShow)
{
	// open up the user's notification icons settings key to listen for
	// notifications/changes as well as the ability to check sub-keys as we
	// we be inspecting/updating each notification entry key
	HKEY baseKey;
	LONG result = RegOpenKeyEx(
		HKEY_CURRENT_USER,
		BUFFER,
		0,
		KEY_NOTIFY | KEY_ENUMERATE_SUB_KEYS,
		&baseKey
	);
	if (result != ERROR_SUCCESS) {
		return 1;
	}

	// before we start monitoring, first update the existing notification
	// icons to make them all visible
	promote_all_sysicons(&baseKey);

	for(;;) {
		// monitor the user's notification icons settings key for changes;
		// this includes new entries and changes to values
		result = RegNotifyChangeKeyValue(
			baseKey,
			TRUE,
			REG_NOTIFY_CHANGE_NAME | REG_NOTIFY_CHANGE_LAST_SET,
			NULL,
			FALSE
		);
		if (result != ERROR_SUCCESS) {
			break;
		}

		// a change event has occurred; ensure every icon is still promoted
		promote_all_sysicons(&baseKey);
	}

	RegCloseKey(baseKey);
	return 0;
}
