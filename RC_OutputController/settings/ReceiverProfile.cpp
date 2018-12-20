
#ifndef RECEIVERPROFILE_DEF
#define RECEIVERPROFILE_DEF

struct ReceiverProfile {
	char* name;
	unsigned char adr[5];
};

const static ReceiverProfile ReceiverProfiles[3] = {
	{"Original Car", "00007"},
	{"Car", "00007"},
	{"Original Spider", "00005"}
};

class PredefReceiverProfiles {
public:
	static ReceiverProfile getProfile(int index) {
		return ReceiverProfiles[index];
	}

	static int maxProfiles() {
		return sizeof(ReceiverProfiles) / sizeof(ReceiverProfile);
	}
};

#endif RECEIVERPROFILE_DEF
