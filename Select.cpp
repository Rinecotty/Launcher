#include "stdafx.h"
#include "Select.h"
Select::Select()
	: selectIndex (0)
	, canInput(true)
	, isPlay(false)
	, leave(false)
	, descArea{}
	, titleArea{}
	, staffArea{}
	, toolArea{}
	, yearArea{}
	, movscale(1)
	, isAnim(false)
	, prevPos (0,0)
	, prevDistance(230)
	, prevCnt(5)//奇数にすること
	, animDuration(0.5)
	, next(Direction::none)
	, nowPos(0, 0)
	, targetPos(0,0)
	, prevSize(200)
	, descPos(0,0)
	, descSize(0,0)
	, movPos(0,0)
	, movSize(0,0)
{
	prevArea.resize(prevCnt + 2);//上の部分と下の部分も入れる.
	nowGameList = &games;//選択しているゲームリストのポインタを初期化.
}
Select::~Select()
{
	audio.stop();
	games.clear();//ゲームのリストをクリア.
}
void Select::Initialize() {
	SelectInit();
}
void Select::SelectInit() {
	leaveTime.Start();
	prevPos = Vec2(150, 600);//プレビューエリアの座標代入.
	movPos = Vec2(750, 100);//動画の表示位置代入.
	movSize = Vec2(1100, 700);//動画の表示サイズ代入.
	descPos = Vec2(750, 800);//説明文の表示位置代入.
	descSize = Vec2(1100, 350);//説明文の表示サイズ代入.
	Load();
	LoadPrev();
	SetRectSize();
}
void Select::InitGenre() {
	for (const auto& game : games) {
		if (game.genre.isEmpty()) continue;//ジャンルが空ならスキップ.

		Array<String> gameGenres = game.genre.split(U',');//ジャンルが複数ある場合は分割して配列に入れる.

		for (auto& g : gameGenres) {
			g = g.trimmed();//前後の空白を削除.
			if (!items.contains(g)) {
				items << g;
			}
		}
	}
}
void Select::InitYear() {
	for (const auto& game : games) {
		if (game.year.empty()) continue;//年度が空ならスキップ.
		if (!years.contains(game.year + U"年")) {
			years << game.year + U"年";
		}
	}
	//for (int i = 0; i < games.size(); i++) {
	//	if (games[i].year== 0)continue;//年度が空ならスキップ.
	//	bool add = true;
	//	for (int n = 0; n < years.size(); n++) {
	//		if (games[i].year == years[n]) {
	//			add = false;
	//			break;
	//		}
	//	}
	//	if (add) {
	//		years << games[i].year;
	//		items << Format(U"{}年"_fmt(games[i].year));
	//	}
	//}
	//
}
void Select::Load() {
#if _DEBUG
	Print << homeDirectory;
#endif
	const FilePath noImgPath = U"data/image/NoImage.png";
	const FilePath backPath = U"data/image/haikei2.png";
	if (FileSystem::Exists(noImgPath) == false) {
		System::MessageBoxOK(U"{} が見つかりませんでした。"_fmt(noImgPath));
	}
	if (FileSystem::Exists(backPath) == false) {
		System::MessageBoxOK(U"{} が見つかりませんでした。"_fmt(backPath));
	}
	noImage = Texture{ noImgPath };//動画等が無かった時の画像読み込み.
	back = Texture{ backPath };//背景画像読み込み.
	LoadGames();//ゲーム読み込み.
	items << U"All";
	years << U"All";
	InitGenre();//ジャンルの初期化.
	InitYear();//年度の初期化.
#if defined TAB
	tab = TabE{ Size{ 160, 50 }, items};
#endif
}
void Select::LoadSortGames() {
	int selectedTab = tab.getActiveTabIndex();
	if (selectedTab <= 0 || selectedTab >= items.size()) {
		nowGameList = &games;//タブのインデックスが不正または0なら全てのゲームを表示&今使っているゲームリストのポインタを更新.
	}
	else {
		selectedGames = SortGames(games, items[selectedTab]);//タブのインデックスに対応する文字列でソート.
		nowGameList = &selectedGames;//今使っているゲームリストのポインタを更新.
	}
	selectIndex = 0;//選択しているゲームのインデックスを初期化.
	LoadPrev();
}
//ゲームのパスがWebページかどうか.
bool Select::IsURL(StringView path) {
	return (path.starts_with(U"http://") || path.starts_with(U"https://"));
}
Array<Game> Select::LoadGames() {
	// ホームディレクトリにあるGameData内アイテムを検索
	for (const FilePath& gameDirectory : FileSystem::DirectoryContents(homeDirectory + U"data/GameData", Recursive::No))	
	{
		// フォルダでない場合はスキップ
		if (not FileSystem::IsDirectory(gameDirectory))
		{
			continue;
		}

		// launcher_info.ini を読み込む
		const FilePath iniPath = (gameDirectory + U"launcher_info.ini");
		const INI ini{ iniPath };

		// iniファイルがなかったらデフォルト値で作成する.
		if (not ini)
		{
			INI tmp;
			tmp.write(U"Game", U"title", U"ゲームのタイトル");
			tmp.write(U"Game", U"image", U"ゲームの画像パス");
			tmp.write(U"Game", U"movie", U"ゲームの説明動画のパス");
			tmp.write(U"Game", U"desc", U"ゲームの説明文");
			tmp.write(U"Game", U"staff", U"ゲーム制作スタッフ");
			tmp.write(U"Game", U"tools", U"制作に使ったツール");
			tmp.write(U"Game", U"genre", U"ジャンル");
			tmp.write(U"Game", U"priority", U"ランチャーでの表示優先度（大きいほど一覧で先頭に表示） ; 整数値");
			tmp.write(U"Game", U"path", U"ゲームのexe名");
			tmp.write(U"Game", U"year", U"制作年度.西暦で書くこと(例：2026)");

			tmp.save(iniPath);
			System::MessageBoxOK(gameDirectory + U" 内にlauncher_info.iniが見つかりませんでした。\nデフォルト値で作成しました。");
		}

		// ゲームの情報を読み込む
		Game game;
		game.title = ini[U"Game.title"];
		game.texture = Texture{ Image{ gameDirectory + ini[U"Game.image"] }.squareClipped(), TextureDesc::Mipped };
		const FilePath moviePath = gameDirectory + ini[U"Game.movie"];
		//if (not FileSystem::Exists(moviePath)) throw Error{ U"{}が見つかりませんでした"_fmt(moviePath) };
		if (FileSystem::Exists(moviePath)) {
			game.mov = VideoTexture{ moviePath,Loop::Yes };//動画の読み込み.
			game.movAudio = Audio{ gameDirectory + ini[U"Game.movie"],Loop::Yes };//音声の読み込み.
		}
		
		game.desc = ini[U"Game.desc"].replaced(U"\\n", U"\n");
		game.staff = ini[U"Game.staff"].replaced(U"\\n", U"\n");
		game.tools = ini[U"Game.tools"];
		game.genre = ini.get<String>(U"Game.genre");
		game.priority = ini.get<int32>(U"Game.priority");

		const String path = game.path = ini[U"Game.path"];
		game.isWebApp = IsURL(path);
		game.path = (game.isWebApp ? path : (gameDirectory + path));
		game.year = ini.get<String>(U"Game.year");
		// ゲームのリストに追加
		games << game;
		nowGameList = &games;
	}

	// プライオリティに基づいてゲームをソート
	return games.sort_by([](const Game& a, const Game& b) { return a.priority > b.priority; });
}
void Select::Update() {
	input.Update();
	const Array<Input> keys = Keyboard::GetAllInputs();
	//何らかの入力があったら.
	if (!keys.isEmpty() || input.IsAnyPadInput()) {
		leaveTime.Reset();//放置時間リセット.
		if (leave) {
			RestoreAudio();
		}
	}
	Leave();

	if (input.IsPressed(Action::Decide) && !isPlay) {
		PlayGame();
	}
	if (process)
	{
		// プロセスが実行中なら
		if (process->isRunning())
		{
			if (!isPlay) isPlay = true;
			// ウィンドウを最小化
			Window::Minimize();
		}
		else // プロセスが終了したら
		{
			// ウィンドウを復帰
			Window::Restore();
			process.reset();
			if (isPlay) {
				leaveTime.Reset();//放置時間リセット.
				if (leave) {
					RestoreAudio();
				}
				isPlay = false;
				audio.play();
			} 
		}
	}
	if (isPlay) return;//プレイフラグがONならこれ以上処理しない.
	if ((*nowGameList).empty()) return;

	if (!canInput) {
		//もし最後の入力から0.5秒経っていたらフラグON.
		if (t_input.ms() > 50) {
			canInput = true;
		}
	}
	if (input.IsPressed(Action::Up) && canInput && !isAnim) {
		canInput = false;
		Anim(Direction::up, 0.5);
		next = Direction::up;
	}
	if (input.IsPressed(Action::Down) && canInput && !isAnim) {
		canInput = false;
		Anim(Direction::down, 0.5);
		next = Direction::down;
	}
#if defined TAB
	if (input.IsPressed(Action::Left))
	{
		tab.advance(-1);
		LoadSortGames();
	}
	if (input.IsPressed(Action::Right))
	{
		tab.advance(+1);
		LoadSortGames();
	}
#endif
	if (input.IsPressed(Action::Sort)) {
		yearIndex = (yearIndex + 1) % years.size();
#if _DEBUG
		Print << U"yearIndex = " << yearIndex;
#endif

	}
	if (isAnim) {
		double t = Min(t_prev.sF(), animDuration) / animDuration;
		double e = EaseOutExpo(t);

		prevPos = nowPos.lerp(targetPos, e);

		if (t >= 1.0) {
			isAnim = false;
			prevPos.y = nowPos.y;
			UpdateIndex(next);
			LoadPrev();
			t_input.start();
		}
	}
}
void Select::Draw() {
	//yearArea.rounded(40, 40, 40, 40).draw();
	back.resized(Scene::Size()).draw(0, 0);
#if defined TAB
	tab.draw(Vec2{ 300, 40 }, fontMgr.GetFont(), TabColor, TabOutlineColor);
#endif
	descArea.rounded(40, 0, 40, 0).draw();
	titleArea.rounded(40, 40, 40, 40).draw();
	staffArea.rounded(40, 40, 40, 40).draw();
	toolArea.rounded(40, 40, 40, 40).draw();
	fontMgr.GetFont()((*nowGameList)[selectIndex].desc).draw(descArea.stretched(-80 * SCALE, -30 * SCALE, -20 * SCALE, -30 * SCALE), Palette::Black);
	DrawPrev();//プレビューを表示.
	
	//Print << items;
}
void Select::LoadPrev() {
	if (audio) {
		audio.stop();
		audio = Audio();
	}										//もし音声を再生中ならストップ.
	//if (mov)mov.release();
	mov = VideoTexture();								//もし動画を再生中ならリリース.

	//プレビューエリアテクスチャー読み込み.
	for (int i = 0; i < prevArea.size(); i++) {
		int32 index = (int32)((*nowGameList).size() + selectIndex + (i - 3)) % (*nowGameList).size();
		if (!(*nowGameList)[index].texture) {
			prevArea[i] = noImage;
		}
		else {
			prevArea[i] = (*nowGameList)[index].texture;
		}
	}

	mov = (*nowGameList)[selectIndex].mov;						//動画.
	audio = (*nowGameList)[selectIndex].movAudio;			//音声.
	//音声または動画が見つからなかったら抜ける.

	if (!mov || !audio) {
		return;
	}
	audio.play();//音声再生.
}


void Select::DrawPrev() {
	int center = (int)prevArea.size() / 2;
	for (int i = 0; i < prevArea.size(); i++) {
		int offset = i - center;
		prevArea[i].fitted(prevSize * SCALE, prevSize * SCALE).rounded(40).drawAt(prevPos.x * SCALE, (prevPos.y + offset * prevDistance ) * SCALE);
	}
	fontMgr.GetFont()(U"タイトル").draw(titleArea.stretched(-10 * SCALE,-10 * SCALE,0 * SCALE,-10 * SCALE), Palette::Black);
	fontMgr.GetFont()(U"制作メンバー").draw(staffArea.stretched(-10 * SCALE,-10 * SCALE,0 * SCALE,-10 * SCALE), Palette::Black);
	fontMgr.GetFont()(U"制作ツール").draw(toolArea.stretched(-10 * SCALE,-10 * SCALE,0 * SCALE,-10 * SCALE), Palette::Black);
	fontMgr.GetFont2()((*nowGameList)[selectIndex].title).draw(titleArea.stretched(-80 * SCALE,-10 * SCALE,0 * SCALE,-10 * SCALE), Palette::Black);
	fontMgr.GetFont2()((*nowGameList)[selectIndex].staff).draw(staffArea.stretched(-80 * SCALE,-10 * SCALE,0 * SCALE,-10 * SCALE), Palette::Black);
	fontMgr.GetFont2()((*nowGameList)[selectIndex].tools).draw(toolArea.stretched(-80 * SCALE,-10 * SCALE,0 * SCALE,-10 * SCALE), Palette::Black);
	Triangle(10, UI::WindowSize.y / 2 - 40, 50, UI::WindowSize.y / 2, 10, UI::WindowSize.y / 2 + 40).draw(Palette::Orange);
	PlayMov();//動画を再生.
}

//アニメーション関連.一回呼ばれたら一回分動かす？
void Select::Anim(Direction dir, double sec) {
	if (!isAnim) {
		isAnim = true;
		nowPos = { prevPos.x, prevPos.y };
		t_prev.restart();
		animDuration = sec;
		if (dir == Direction::up) {
			targetPos = { prevPos.x, prevPos.y + prevDistance };
		}
		else {
			targetPos = { prevPos.x, prevPos.y - prevDistance };
		}
	}
}

/// @brief プレビューアイコン次のやつにする.
/// @param upかdownか
void Select::UpdateIndex(Direction dir) {
	switch (dir) {
	case Direction::up:
		selectIndex--;
		if (selectIndex < 0) {
			selectIndex = (int32)(*nowGameList).size() - 1;
		}
		break;
	case Direction::down:
		selectIndex++;
		if (selectIndex >= (int32)(*nowGameList).size()) {
			selectIndex = 0;
		}
		break;
	default:
		System::MessageBoxOK(U"UpdateIndexが想定外");
	}
}

void Select::PlayMov() {
	if (isPlay) return;
	if (!mov) {
		noImage.fitted(movSize.x, movSize.y).draw(movPos.x * SCALE, movPos.y * SCALE);
		return;
	}
	ClearPrint();
	//動画の再生位置.
	const double videoTime = mov.posSec();
	//音声の再生位置.
	const double audioTime = audio.posSec();
	//再生位置（秒）/　動画の長さ（秒）
#if _DEBUG
	Print << U"{:.2f}/{:.2f}"_fmt(mov.posSec(), mov.lengthSec());
	Print << mov.size().x;
	Print << mov.size().y;
#endif
	//動画の再生位置と音声の再生位置の差が0.1秒以上ある場合
	if (0.1 < AbsDiff(audioTime, videoTime)) {
		audio.seekTime(videoTime);

	}
	//動画の時間を進める
	mov.advance();

	mov.fitted(movSize.x,movSize.y).draw(movPos.x * SCALE,movPos.y * SCALE);
}
void Select::SetRectSize() {
	int32 firstY = 200;
	int32 w = 450;
	int32 h = 180;
	descArea = RectF{ descPos.x * SCALE,descPos.y * SCALE,descSize.x * SCALE,descSize.y* SCALE };
	titleArea = RectF{ 275 * SCALE,firstY * SCALE,w * SCALE,h* SCALE };
	staffArea = RectF{ 275 * SCALE,(firstY + 200) * SCALE,w * SCALE,h * SCALE };
	toolArea = RectF{ 275 * SCALE,(firstY + 400) * SCALE,w * SCALE,h * SCALE };
	yearArea = RectF{ 10 * SCALE,10 * SCALE,300 * SCALE,200 * SCALE };
}
void Select::PlayGame() {
	if (FileSystem::Exists((*nowGameList)[selectIndex].path)) {
		process = ChildProcess{ (*nowGameList)[selectIndex].path };
		audio.stop();
		mov.reset();

	}
}

Array<Game> Select::SortGames(Array<Game> gameList,String moji) {
	Array<Game> result;
	for (const auto& game : games) {
		//ジャンル検索.
		if (game.genre.contains(moji)) {
			result << game;
		}
	}
	////年度での検索を試みる(最初に数字があるかどうかしか見てない).
	//try {
	//	String yearStr = moji.remove(U"年");
	//	int32 num = Parse<int32>(yearStr);
	//	for (int i = 0; i < games.size(); i++) {
	//		if (games[i].year == num) {
	//			result << games[i];
	//		}
	//	}
	//}
	////数字がなかったらジャンル検索する.
	//catch(const ParseError& error){
	//	for (int i = 0; i < games.size(); i++) {
	//		if (games[i].genre.contains(moji)) {
	//			result << games[i];
	//		}
	//	}
	//}
	return result;
}
void Select::Leave() {
	//if (leave)return;//すでに放置中なら何もしない.
	uint64 tmpTime = leaveTime.GetPassTime();//経過時間取得.
	//１分経ったらかつ放置してないなら.
	if (tmpTime > UI::AudioLeaveTime * 1000000 && !leave) {
		//音楽止める.
		audio.stop();
		leave = true;
	}
	//3分経ったらかつプロセスが実行中なら.
	if (tmpTime > UI::AppLeaveTime * 1000000) {
		if (process) {
			process->terminate();//ゲームを強制終了.
			process.reset();
			RestoreWindow();
		}
	}
}
void Select::RestoreWindow() {
	//windowが最小化されていたら復帰.
	if (Window::GetState().minimized) {
		Window::Restore();
		isPlay = false;
	}
	leave = true;
}
void Select::RestoreAudio() {
	//動画再生中なら音声の時間を合わせて復帰.
	if (mov) {
		audio.seekTime(mov.posSec());
		audio.play();
		leave = false;
	}
}
