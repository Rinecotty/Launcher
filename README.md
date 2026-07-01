# OHO Launcher
## 概要
### 制作のきっかけ
- 学校の筐体に入れてゲームを展示できるランチャーを作成しようと思い立ったから

### 主な機能・説明
> **設定ファイルの自動作成**
- アプリケーション起動時、実行exeのあるフォルダに`config.ini`、`GameData`内に入れたゲームディレクトリ内に設定ファイル`launcher_info.ini`がなければ自動作成します。
<details>
<summary> ディレクトリ構成</summary>

```
OHOLauncher/
├─Launcher.exe
├─config.ini
├─data
│  ├─fonts
│  ├─GameData
│  └─image
└─Intermediate
```
</details>

> **iniファイルでアプリの設定やゲームの情報を管理**
- ウィンドウサイズ、フルスクリーンにするか、フレームレスで表示するかどうか、放置と判定する時間を`config.ini`内より自由に変更できます。

> **OHOLauncherの仕様**
- ゲームセレクト画面起動時、`config.ini`で設定した時間放置すると紹介動画の音が消えるようになっています。またゲーム実行時、一定時間がたつとアプリが落ちてランチャー画面に戻るようになっています。

- `data/GameData`内にゲームフォルダを追加し、その中に紹介動画、アイコン画像、exe＆その他必要なファイルを入れ、`launcher_info.ini`内で設定することで実行画面で情報を表示できます。

> **各iniファイルの説明**

**〇ゲーム情報設定ファイル`launchar_info.ini`**

<summary> launchar_info.iniデフォルト値</summary>

```
[Game]
title = ゲームのタイトル
image = ゲームの画像パス
movie = ゲームの説明動画のパス
desc = ゲームの説明文
staff = ゲーム制作スタッフ
tools = 制作に使ったツール
genre = ジャンル
priority = ランチャーでの表示優先度（大きいほど一覧で先頭に表示） ; 整数値
path = ゲームのexe名
year = 制作年度.西暦で書くこと(例：2026)
```
|名前|説明|
|--|--|
title|ゲームのタイトル名
image|アイコンに表示する画像のファイル名
movie|プレビューエリアに表示する動画のファイル名
desc|ゲームの説明文。`\n`で改行
staff|ゲーム制作スタッフ。`\n`で改行
tools|ゲーム制作ツールをかく
genre|ゲームのジャンルを書く。複数登録対応。`,`で区切ること
priority|ゲームの表示優先度。大きいほど一覧で先頭に表示されます。整数値にすること
path|ゲームのexe名
year|制作年度


**〇アプリケーション設定ファイル`config.ini`**
<summary>config.iniデフォルト値</summary>

```
[Window]
width = 1920
height = 1200
Fullscreen = 1
Frameless = 0

[Config]
AppLeaveTime = 30
AudioLeaveTime = 30
```
|名前|説明|
|--|--|
width|画面横幅
height|画面縦幅
Fullscreen|1でフルスクリーン表示、0でウィンドウ表示
Frameless|1でフレームレス表示、0でフレームありで表示
AppLeaveTime|アプリ起動時に放置したと判定する時間(秒)
AudioLeaveTime|ゲームセレクト画面で放置したと判定する時間(秒)

## 使用言語・フレームワーク
C++、OpenSiv3D_0.6.16

## 今後追加・改善予定
- ジャンルごとにソートする機能は実装済みだったがジャンルが多すぎると画面からはみ出す・ジャンル名が長すぎると表示タブからはみ出すバグがあるので現在未実装。今後直す予定
- 年度ごとにソートする機能
- ゲーム一覧を切り替えるときに先行入力を受け付ける
- 起動時に毎回ゲームデータを読み込んでいるためデータ数が多くなると起動が遅くなるため改善予定