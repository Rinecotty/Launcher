#include "stdafx.h"
#include "Select.h"
Select::Select()
	: selectIndex (1)
	, canInput(true)
	, isPlay(false)
	, descArea{}
	, movscale(1)
	, isAnim(false)
	, prevPos (0,0)
	, prevDistance(350)
{
	
}
Select::~Select()
{
	/*for (int i = 0; i < games.size(); i++) {
		games[i].texture.release();
		games[i].mov.release();
		games[i].movAudio.release();
	}*/
	audio.stop();
	games.clear();
}
void Select::Initialize() {
	SelectInit();
}
void Select::SelectInit() {
	prevPos = Point(500, 600);//プレビューエリアの座標代入.
	Load();
	LoadPrev();
	SetRectSize();
}
void Select::Load() {
	Print << homeDirectory;
	const FilePath noImgPath = U"data/image/NoImage.png";
	noImage = Texture{ noImgPath };//動画等が無かった時の画像読み込み.	
	LoadGames();//ゲーム読み込み.
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
			tmp.write(U"Game", U"mouse", U"マウス操作 ; true または false");
			tmp.write(U"Game", U"keyboard", U"キーボード操作 ; true または false");
			tmp.write(U"Game", U"gamepad", U"ゲームパッド操作 ; true または false");
			tmp.write(U"Game", U"priority", U"ランチャーでの表示優先度（大きいほど一覧で先頭に表示） ; 整数値");
			tmp.write(U"Game", U"path", U"ゲームのexe名");

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
		game.staff = ini[U"Game.staff"];
		game.tools = ini[U"Game.tools"];
		game.useMouse = ini.get<bool>(U"Game.mouse");
		game.useKeyboard = ini.get<bool>(U"Game.keyboard");
		game.useGamepad = ini.get<bool>(U"Game.gamepad");
		game.priority = ini.get<int32>(U"Game.priority");

		const String path = game.path = ini[U"Game.path"];
		game.isWebApp = IsURL(path);
		game.path = (game.isWebApp ? path : (gameDirectory + path));

		// ゲームのリストに追加
		games << game;
	}

	// プライオリティに基づいてゲームをソート
	return games.sort_by([](const Game& a, const Game& b) { return a.priority > b.priority; });
}
void Select::Update() {
	if (KeyEnter.down()) {
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
				isPlay = false;
				audio.play();
			} 
		}
	}
	if (games.empty()) return;

	if (!canInput) {
		//もし最後の入力から0.5秒経っていたらフラグON.
		if (t_input.ms() > 50) {
			canInput = true;
		}
	}
	if (KeyUp.down() && canInput) {
		canInput = false;
		Anim(Direction::up, 0.5);
		next = Direction::up;
	}
	if (KeyDown.down() && canInput) {
		canInput = false;
		Anim(Direction::down, 0.5);
		next = Direction::down;
	}
	if (isAnim) {
		double t = Min(t_prev.sF(), animDuration) / animDuration;
		double e = EaseOutExpo(t);

		prevPos = nowPos.lerp(targetPos, e);

		if (t >= 1.0) {
			isAnim = false;
			UpdateIndex(next);
			LoadPrev();
			t_input.start();
		}
	}
}
void Select::Draw() {
	DrawPrev();//プレビューを表示.
	descArea.rounded(40, 0, 40, 0).draw();
	fontMgr.GetFont()(games[selectIndex].desc).draw(descArea.stretched(-80, -30, -20, -30), Palette::Black);
}
void Select::LoadPrev() {
	Print << games.size();
	if (audio) {
		audio.stop();
		audio = Audio();
		//audio.release();
	}										//もし音声を再生中ならストップ.
	//if (mov)mov.release();
	mov = VideoTexture();

	if (!games[selectIndex].texture) {
		prevImage = noImage;
	}
	else {
		prevImage = games[selectIndex].texture;	//Preview.
	}

	//selectIndexがゲーム配列サイズちょうどなら.
	if (selectIndex == (int32)games.size() - 1) {
		if (!games[0].texture) {
			prev2 = noImage;
		}
		else prev2 = games[0].texture;
		if (!games[selectIndex - 1].texture) {
			prev3 = noImage;
		}else prev3 = games[selectIndex - 1].texture;
		Print << U"size-1";
	}
	//selectIndexが0なら.
	else if (selectIndex == 0) {
		if (!games[games.size() - 1].texture) {
			prev2 = noImage;
		}
		else prev2 = games[games.size() - 1].texture;
		if (!games[selectIndex + 1].texture) {
			prev3 = noImage;
		}
		else prev3 = games[selectIndex + 1].texture;
	}
	//selectIndexがそれ以外なら.
	else {
		if (!games[selectIndex - 1].texture) {
			prev2 = noImage;
		}
		else prev2 = games[selectIndex - 1].texture;
		if (!games[selectIndex + 1].texture) {
			prev3 = noImage;
		}
		else prev3 = games[selectIndex + 1].texture;
		Print << U"LoadPrev:else";
	}
	mov = games[selectIndex].mov;						//動画.
	audio = games[selectIndex].movAudio;			//音声.
	//音声または動画が見つからなかったら抜ける.
	if (!mov || !audio) {
		return;
	}
	audio.play();//音声再生.
}

void Select::DrawPrev() {
	prevImage.fitted(300 * SCALE,300 * SCALE).rounded(40).drawAt(prevPos.x * SCALE, prevPos.y * SCALE);//600
	prev2.fitted(300 * SCALE, 300 * SCALE).rounded(40).drawAt(prevPos.x * SCALE, (prevPos.y - prevDistance) * SCALE);//250
	prev3.fitted(300 * SCALE, 300 * SCALE).rounded(40).drawAt(500 * SCALE, (prevPos.y + prevDistance) * SCALE);//500
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
			targetPos = { prevPos.x, prevPos.y - 350 };
		}
		else {
			targetPos = { prevPos.x, prevPos.y + 350 };
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
			selectIndex = (int32)games.size() - 1;
		}
		break;
	case Direction::down:
		selectIndex++;
		if (selectIndex >= (int32)games.size()) {
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
		noImage.scaled(0.6).draw(750 * SCALE, 50 * SCALE);
		return;
	}
	ClearPrint();
	//動画の再生位置.
	const double videoTime = mov.posSec();
	//音声の再生位置.
	const double audioTime = audio.posSec();
	//再生位置（秒）/　動画の長さ（秒）
	Print << U"{:.2f}/{:.2f}"_fmt(mov.posSec(), mov.lengthSec());
	Print << mov.size().x;
	Print << mov.size().y;
	//動画の再生位置と音声の再生位置の差が0.1秒以上ある場合
	if (0.1 < AbsDiff(audioTime, videoTime)) {
		audio.seekTime(videoTime);
	}

	//動画の時間を進める
	mov.advance();

	mov.fitted(1152,720).draw(750 * SCALE,50 * SCALE);
}
void Select::SetRectSize() {
	descArea = RectF{ 750 * SCALE,800 * SCALE,1150 * SCALE,350* SCALE };
}
void Select::PlayGame() {
	process = ChildProcess{ games[selectIndex].path };
	audio.stop();
	mov.reset();
}
