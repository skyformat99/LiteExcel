#include "XString.h"
#include <stdlib.h>
#include <string.h>

XString::XString(const char *str) {
	mLen = (str == NULL ? 0 : strlen(str));
	mBuffer = (char *)malloc(mLen + 1);
	*mBuffer =  0;
	if (str != NULL) {
		strcpy(mBuffer, str);
	}
}
XString::XString(const XString &rh) {
	if (mLen != rh.mLen) {
		mBuffer = (char *)realloc(mBuffer, rh.mLen + 1);
	}
	strcpy(mBuffer, rh.mBuffer);
	mLen = rh.mLen;
}

XString::~XString() {
	if (mBuffer != NULL) {
		free(mBuffer);
	}
}
XString& XString::operator=(const XString &rh) {
	if (mLen != rh.mLen) {
		mBuffer = (char *)realloc(mBuffer, rh.mLen + 1);
	}
	strcpy(mBuffer, rh.mBuffer);
	mLen = rh.mLen;
	return *this;
}
char * XString::str() {
	return mBuffer;
}
int XString::length() {
	return mLen;
}
