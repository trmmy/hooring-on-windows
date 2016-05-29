/*
ClipCursor	�}�E�X�J�[�\���̈ړ��\�Ȕ͈͂��A�w�肳�ꂽ�����`�̓����ɐ������܂��B
CopyCursor	�w�肳�ꂽ�}�E�X�J�[�\�����R�s�[���܂��B
CreateCursor	�w�肳�ꂽ�T�C�Y�A�r�b�g�p�^�[���A�z�b�g�X�|�b�g�����}�E�X�J�[�\�����쐬���܂��B
DestroyCursor	CreateCursor �֐����g���č쐬�����}�E�X�J�[�\����j�����A�}�E�X�J�[�\���p�Ɏg���Ă�����������������܂��B
GetClipCursor	�}�E�X�J�[�\���̈ړ��\�Ȕ͈͂ɑ�������X�N���[�����W���擾���܂��B
GetCursor	���݂̃}�E�X�J�[�\���̃n���h�����擾���܂��B
GetCursorInfo	�O���[�o���J�[�\���Ɋւ�������擾���܂��B
GetCursorPos	�}�E�X�J�[�\���̌��݂̈ʒu�ɑ�������X�N���[�����W���擾���܂��B
LoadCursor	�w�肳�ꂽ�}�E�X�J�[�\���̃��\�[�X�����[�h���܂��B
LoadCursorFromFile	�t�@�C�����ɋL�^����Ă���f�[�^�Ɋ�Â��āA�}�E�X�J�[�\�����쐬���܂��B
SetCursor	�}�E�X�J�[�\���̌`���ݒ肵�܂��B
SetCursorPos	�}�E�X�J�[�\���̈ʒu���A�w�肳�ꂽ�X�N���[�����W�Ɉړ����܂��B
SetSystemCursor	�A�v���P�[�V�����ɂ��V�X�e���J�[�\���̕ύX���\�ɂ��܂��B
ShowCursor	�}�E�X�J�[�\����\���܂��͔�\���ɂ��܂��B
*/

/*
BOOL ClipCursor(
  CONST RECT *lpRect  // �͈͂𐧌�����
);

HCURSOR CopyCursor(
  HCURSOR pcur   // �R�s�[�������}�E�X�J�[�\���̃n���h��
);

HCURSOR CreateCursor(
  HINSTANCE hInst,         // �A�v���P�[�V�����C���X�^���X�̃n���h��
  int xHotSpot,            // �z�b�g�X�|�b�g�� x ���W
  int yHotSpot,            // �z�b�g�X�|�b�g�� y ���W
  int nWidth,              // �J�[�\���̕�
  int nHeight,             // �J�[�\���̍���
  CONST VOID *pvANDPlane,  // AND �}�X�N�̔z��
  CONST VOID *pvXORPlane   // XOR �}�X�N�̔z��
);

BOOL DestroyCursor(
  HCURSOR hCursor   // �j���������J�[�\���̃n���h��
);

BOOL GetClipCursor(
  LPRECT lpRect   // �X�N���[�����W
);

HCURSOR GetCursor(VOID);

BOOL GetCursorInfo(
  PCURSORINFO pci
);

BOOL GetCursorPos(
  LPPOINT lpPoint   // �J�[�\���̈ʒu
);

HCURSOR LoadCursor(
  HINSTANCE hInstance,  // �A�v���P�[�V�����C���X�^���X�̃n���h��
  LPCTSTR lpCursorName  // ���O�܂��̓��\�[�X�̎��ʎq
);

HCURSOR LoadCursorFromFile(
  LPCTSTR lpFileName  // �t�@�C���܂��͎��ʎq
);

HCURSOR SetCursor(
  HCURSOR hCursor   // �J�[�\���̃n���h��
);

BOOL SetCursorPos(
  int X,  // �����ʒu
  int Y   // �����ʒu
);

BOOL SetSystemCursor (
  HCURSOR hcur,  // �V�����ݒ肷��}�E�X�J�[�\���̃n���h��
  DWORD id       // �V�X�e���J�[�\���̎��ʎq
);

int ShowCursor(
  BOOL bShow   // �J�[�\���̉����
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
