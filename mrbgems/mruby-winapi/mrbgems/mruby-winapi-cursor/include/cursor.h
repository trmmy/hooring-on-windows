/*
ClipCursor	マウスカーソルの移動可能な範囲を、指定された長方形の内側に制限します。
CopyCursor	指定されたマウスカーソルをコピーします。
CreateCursor	指定されたサイズ、ビットパターン、ホットスポットを持つマウスカーソルを作成します。
DestroyCursor	CreateCursor 関数を使って作成したマウスカーソルを破棄し、マウスカーソル用に使われていたメモリを解放します。
GetClipCursor	マウスカーソルの移動可能な範囲に相当するスクリーン座標を取得します。
GetCursor	現在のマウスカーソルのハンドルを取得します。
GetCursorInfo	グローバルカーソルに関する情報を取得します。
GetCursorPos	マウスカーソルの現在の位置に相当するスクリーン座標を取得します。
LoadCursor	指定されたマウスカーソルのリソースをロードします。
LoadCursorFromFile	ファイル内に記録されているデータに基づいて、マウスカーソルを作成します。
SetCursor	マウスカーソルの形状を設定します。
SetCursorPos	マウスカーソルの位置を、指定されたスクリーン座標に移動します。
SetSystemCursor	アプリケーションによるシステムカーソルの変更を可能にします。
ShowCursor	マウスカーソルを表示または非表示にします。
*/

/*
BOOL ClipCursor(
  CONST RECT *lpRect  // 範囲を制限する
);

HCURSOR CopyCursor(
  HCURSOR pcur   // コピーしたいマウスカーソルのハンドル
);

HCURSOR CreateCursor(
  HINSTANCE hInst,         // アプリケーションインスタンスのハンドル
  int xHotSpot,            // ホットスポットの x 座標
  int yHotSpot,            // ホットスポットの y 座標
  int nWidth,              // カーソルの幅
  int nHeight,             // カーソルの高さ
  CONST VOID *pvANDPlane,  // AND マスクの配列
  CONST VOID *pvXORPlane   // XOR マスクの配列
);

BOOL DestroyCursor(
  HCURSOR hCursor   // 破棄したいカーソルのハンドル
);

BOOL GetClipCursor(
  LPRECT lpRect   // スクリーン座標
);

HCURSOR GetCursor(VOID);

BOOL GetCursorInfo(
  PCURSORINFO pci
);

BOOL GetCursorPos(
  LPPOINT lpPoint   // カーソルの位置
);

HCURSOR LoadCursor(
  HINSTANCE hInstance,  // アプリケーションインスタンスのハンドル
  LPCTSTR lpCursorName  // 名前またはリソースの識別子
);

HCURSOR LoadCursorFromFile(
  LPCTSTR lpFileName  // ファイルまたは識別子
);

HCURSOR SetCursor(
  HCURSOR hCursor   // カーソルのハンドル
);

BOOL SetCursorPos(
  int X,  // 水平位置
  int Y   // 垂直位置
);

BOOL SetSystemCursor (
  HCURSOR hcur,  // 新しく設定するマウスカーソルのハンドル
  DWORD id       // システムカーソルの識別子
);

int ShowCursor(
  BOOL bShow   // カーソルの可視状態
);

*/

/*
typedef struct {
  DWORD   cbSize;
  DWORD   flags;
  HCURSOR hCursor;
  POINT   ptScreenPos;
} CURSORINFO

*/

#ifndef RESOURCE_CURSOR_H
#define RESOURCE_CURSOR_H

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/winapi.h>

#endif // RESOURCE_CURSOR_H
