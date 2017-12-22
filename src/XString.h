#pragma once

class XString {
public:
	XString(const char *str = "");
	XString(const XString &rh);
	XString &operator=(const XString &rh);

	char *str(); // always not NULL
	int length();

	~XString();
protected:
	char *mBuffer;
	int mLen;
};

