#pragma once
#include "BaseScene.h"
#include "Application.h"
#include "FontMgr.h"
#include "Tab.h"
//ゲーム情報入れる構造体.
struct Game {
	String title;							//ゲームのタイトル.
	FilePath path;						//ゲーム実行ファイル.
	bool isWebApp = false;		//Webブラウザで起動.
	Texture texture;					//ゲームの画像.
	VideoTexture mov;				//ゲームの動画オブジェクト.
	Audio movAudio;					//ゲームの音声オブジェクト.
	String desc;							//ゲームの説明文.
	String staff;						//ゲームの開発スタッフ.
	String tools;						//ゲームの開発ツール.
	String genre;						//ゲームのジャンル.
	int32 year;							//ゲーム制作年度.2026や2025など.
	bool useMouse = false;		//マウスを使用するか.
	bool useKeyboard = false;	//キーボードを使用するか.
	bool useGamepad = false;	//ゲームパッドを使用するか.
	int32 priority = 0;				//ランチャー表示優先度.
};


class Select  {
private:
	//方向の定義.
	enum Direction {
		up,
		down,
		left,
		right,
		none
	};
	
	FontMgr& fontMgr = FontMgr::GetInstance();
	const FilePath homeDirectory = FileSystem::CurrentDirectory();//ホームディレクトリ格納.
# pragma region プレビューエリア系変数
	Texture noImage;
	Array<Texture> prevArea;
	int32 prevCnt;//プレビューエリアの数.
	Vec2 prevPos;//プレビューアイコンの真ん中の座標.
	Vec2 movPos;//動画の表示位置.
	Vec2 movSize;//動画の表示サイズ.
	Vec2 descPos;//説明文の表示位置.
	Vec2 descSize;//説明文の表示サイズ.
	int prevDistance;//プレビューアイコンの間の距離.
	int32 prevSize;//プレビューアイコンのサイズ.
	Stopwatch t_prev;//プレビューアイコン用タイマー.
	Vec2 nowPos;//今の位置記録.
# pragma endregion
#pragma region セレクトアニメーション
	Vec2 targetPos;//アニメーションの目標位置記録.
	Direction next;//セレクトアニメーション方向保持.
	double animDuration;//セレクトアニメーション遷移時間.
#pragma endregion
#pragma region タブ関連
	const Font font{ FontMethod::MSDF, 48, Typeface::Heavy };
	Array<String> items = {};
	Array<int32> years = {};
	ColorF TabColor{ 0.2, 0.5, 0.9 };
	ColorF TabOutlineColor{ 0.5 };
	ColorF ContentColor{ 0.5 };
	TabE tab{ Size{ 160, 50 }, items };
#pragma endregion
	
	int32 selectIndex = 0;	//選択しているタイルのインデックス.
	RectF descArea;				//説明エリア.

	Array<Game> games;		//ゲームのリスト.
	Array<Game> selectedGames;	//優先度でソートしたゲームのリスト.
	Array<Game>* nowGameList;//今使っているゲームリストのポインタ.
	VideoTexture mov;	//動画オブジェクト.
	Audio audio;	//動画再生時のAudioオブジェクト.
	double movscale;	//動画のサイズ.
	Stopwatch t_input;//入力制限用のストップウォッチ.
	bool canInput;//入力可能か.
	bool isPlay;//ゲーム中か.
	bool isAnim;//アニメーション.

	Optional<ChildProcess> process;//実行中のゲームプロセス.
public:
	Select();
	~Select();
	void Initialize() ;
	void InitGenre();	//ジャンル情報からジャンルリストを作成.
	void InitYear();//制作年度情報から年度リストを作成.
	void SelectInit();//初期化の中身ここに書く.
	void Finalize(){};
	void Update();
	void Draw();
	void DrawPrev();//プレビューアイコンの表示.
	void UpdateIndex(Direction dir);//セレクトエリアを次にする(引数によって上か下か判断).
	void Anim(Direction dir,double sec);	//選択エリアのアニメーション.

	void SetRectSize();//UIサイズ等変更する.
	void LoadPrev();	//現在選択中のインデックスから動画と音声をロード？.
	
	void PlayMov();
	void PlayGame();//ゲームを開始.
	Array<Game> SortGames(Array<Game> gameList,String moji);//ゲームの検索結果を返す(Array<Game>型).

	void Load();
	void LoadSortGames();
	
	Array<Game>LoadGames();
	bool IsURL(StringView path);
};
