#pragma once
class FontMgr {
private:
	FontMgr();
	~FontMgr() {}
	Font font;
	Font font2;
	FilePath fontPath = FileSystem::CurrentDirectory() + U"data/fonts/craftmincho.otf";
public:
	static FontMgr& GetInstance();
	void Load();
	const Font& GetFont() const;
	const Font& GetFont2() const;
};
