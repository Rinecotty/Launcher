#include"stdafx.h"
#include "FontMgr.h"
FontMgr::FontMgr() {
	Load();
}
void FontMgr::Load() {
	font = { FontMethod::MSDF, 48,fontPath};
}
FontMgr& FontMgr::GetInstance() {
	static FontMgr instance;
	return instance;
}
const Font& FontMgr::GetFont() const{
	return font;
}
