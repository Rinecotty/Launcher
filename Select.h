#pragma once
#include "BaseScene.h"
#include "Application.h"
#include "FontMgr.h"
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
	Texture noImage;
	Array<Texture> prevArea;
	int32 prevCnt;//プレビューエリアの数.
	Vec2 prevPos;//プレビューアイコンの一番上の座標.
	int prevDistance;//プレビューアイコンの間の距離.
	Stopwatch t_prev;//プレビューアイコン用タイマー.
	Vec2 nowPos;//今の位置記録.
	Vec2 targetPos;//アニメーションの目標位置記録.
	Direction next;//セレクトアニメーション方向保持.
	double animDuration;//セレクトアニメーション遷移時間.

	int32 selectIndex = 0;	//選択しているタイルのインデックス.


	RectF descArea;

	Array<Game> games;		//ゲームのリスト.
	VideoTexture mov;	//動画オブジェクト.
	Audio audio;	//動画再生時のAudioオブジェクト.
	double movscale;	//動画のサイズ.
	Stopwatch t_input;//入力制限用のストップウォッチ.
	bool canInput;
	bool isPlay;//ゲーム中か.
	bool isAnim;//アニメーション.

	Optional<ChildProcess> process;//実行中のゲームプロセス.
public:
	Select();
	~Select();
	void Initialize() ;
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

	void Load();
	
	Array<Game>LoadGames();
	bool IsURL(StringView path);
};
