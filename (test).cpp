
// Include statements
#include <windows.h>
#include <windef.h>
#include <atlstr.h>
#include <shlobj.h>
#include "resource.h"
#include "program.h"
#include "class.h"
#include "function.h"

// Global objects
extern handleitem Handle;

// Run a snippet of test code
void Test() {

}



/*
// Hash data
class hashitem {
public:

	HCRYPTHASH hash;

	bool Hash(byte *data, DWORD size);
	bool Done(string *value);

	// New
	hashitem() {
		hash = NULL;
	}
};

bool hashitem::Hash(byte *data, DWORD size) {

	if (!Handle.provider) {
		if (!CryptAcquireContext(&Handle.provider, NULL, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) return false;
	}

	if (!hash) {
		if (!CryptCreateHash(Handle.provider, CALG_SHA1, 0, 0, &hash)) return false
	}

	if (!CryptHashData(hash, data, size, 0)) return false;

	return true;
}

bool hashitem::Done(string *s) {

	*s = L"";

	BYTE value[20]; // This will hold our SHA-1 hash
	DWORD valuesize = 20; // Size of output, and a count variable for a for loop
	WCHAR bay[MAX_PATH];

	if (CryptGetHashParam(hash, HP_HASHVAL, value, &valuesize, 0)) { // Extract the hash
		for (int i = 0; i < 20; i++) { // Format the hash into a big endian, hexadecimal string
			wsprintf(bay + (i * 2), L"%02x", value[i]); // Each call writes two wide characters and a null terminator
		}
		*s = bay;
	}
	CryptDestroyHash(hash);
	return is(*s);
}
*/

/*
string hash(byte *data, DWORD size) {

	HCRYPTPROV provider; // Handle to our context
	HCRYPTHASH hash; // Handle to our hash
	BYTE value[20]; // This will hold our SHA-1 hash
	DWORD valuesize = 20; // Size of output, and a count variable for a for loop
	WCHAR bay[MAX_PATH];
	string s;

	if (CryptAcquireContext(&provider, NULL, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) { // Initiate usage of the functions
		if (CryptCreateHash(provider, CALG_SHA1, 0, 0, &hash)) { // Create a SHA1 hash
			if (CryptHashData(hash, data, size, 0)) { // Update the hash, (process our password)
				if (CryptGetHashParam(hash, HP_HASHVAL, value, &valuesize, 0)) { // Extract the hash
					for (int i = 0; i < 20; i++) { // Format the hash into a big endian, hexadecimal string
						wsprintf(bay + (i * 2), L"%02x", value[i]); // Each call writes two wide characters and a null terminator
					}
					s = bay;
				}
			}
			CryptDestroyHash(hash);
		}
		CryptReleaseContext(provider, 0);
	}
	return s;
}
*/






















