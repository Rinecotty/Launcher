#include"stdafx.h"
#include "FontMgr.h"
FontMgr::FontMgr() {
	Load();
}
void FontMgr::Load() {
	font = { FontMethod::MSDF, 48,fontPath};
	font2 = { FontMethod::MSDF, 24,fontPath };
}
FontMgr& FontMgr::GetInstance() {
	static FontMgr instance;
	return instance;
}
const Font& FontMgr::GetFont() const{
	return font;
}

const Font& FontMgr::GetFont2() const {
	return font2;
}
