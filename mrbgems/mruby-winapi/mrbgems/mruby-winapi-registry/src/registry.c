#include <mruby.h>
#include <mruby/winapi.h>

/* key => directory */
/* value => file */
/* data => content   */

/* Registry::CurrentUser */
/* Registry::LocalMachine */
/* Registry::Users */
/* Registry.exists? 'key' */

/* _reg = Registry.new */

/* reg = Registry.new('key', 'mode') */
/* ... */
/* reg.close */

/* reg = Registry.open('key', 'mode') { |r| */
/*   reg.each do |key, value| */
/*   end */
/*   reg[key] */
/*   reg[key] = _reg */
/* ... */
/* } */

/* HKEY_CLASSES_ROOT (0x80000000) */
/* �A�v���P�[�V�����̊֘A�Â��ECLSID�Ǘ��B */
/* HKEY_CURRENT_USER (0x80000001) */
/* ���O�I�����[�U�̃v���t�@�C���B */
/* HKEY_LOCAL_MACHINE (0x80000002) */
/* �V�X�e���S�̂̐ݒ�B HARDWARE�T�u�L�[�̉��̓V�X�e���N�����ɍ̏W�����B */
/* HKEY_USERS (0x80000003) */
/* ���[�U�S�̂̃v���t�@�C���B �������A�ʏ탍�O�I�������[�U�̂��̂ƃf�t�H���g�݂̂����[�h����Ă���B */
/* HKEY_PERFORMANCE_DATA (0x80000004) */
/* NT�p�t�H�[�}���X�f�[�^�B API�Ăяo�����Ƀp�t�H�[�}���X�擾�pDLL�ɂ��̏W�����B */
/* HKEY_CURRENT_CONFIG (0x80000005) */
/* ���݂̃V�X�e���ݒ�B */
/* HKEY_DYN_DATA (0x80000006) */
/* 95���\�[�X�Ȃǂ̕ϓ��f�[�^�B */

mrb_value
mrb_registry_open_key(mrb_state* mrb, mrb_value self)
{

/* ONG RegOpenKeyEx(HKEY hOpenKey, LPCTSTR lpSubKey, NULL, REGSAM samDesired, PHKEY phkResult); */
/* �@hOpenKey �c ���łɊJ���ꂽ�L�[���w�肵�܂��B��������ɂ��āAlpSubKey���J���܂��B�n�C�u���w��ł��܂��B */
/* �@lpSubKey �c ���ꂩ��J�������T�u�L�[���w�肵�܂��B�擪�� \ �ł͂��߂Ă͂����܂���B */
/* �@samDesired �c �J�����L�[�ւ̃A�N�Z�X�������w�肵�܂��B */

/* KEY_CREATE_LINK(0x0020) �c �V���{���b�N�����N���쐬�ł��܂��B */
/* KEY_CREATE_SUB_KEY(0x0004) �c �T�u�L�[���쐬�ł��܂��B */
/* KEY_ENUMERATE_SUB_KEYS(0x0008) �c �T�u�L�[��񋓂ł��܂��B */
/* KEY_EXECUTE �c �ǂݎ��A�N�Z�X�ł��܂��B */
/* KEY_NOTIFY(0x0010) �c �ύX�̒ʒm�i�L�[�ɕύX��������ꂽ�Ƃ��ɁA���̂��Ƃ�ʒm����悤�v���j���ł��܂��B */
/* KEY_QUERY_VALUE(0x0001) �c �T�u�L�[�̃f�[�^��₢���킹�邱�Ƃ��ł��܂��B */
/* KEY_SET_VALUE(0x0002) �c �T�u�L�[�̃f�[�^��ݒ肷�邱�Ƃ��ł��܂��B */
/* KEY_ALL_ACCESS �c �Ȃ�ł��ł��܂�(KEY_QUERY_VALUE + KEY_ENUMERATE_SUB_KEYS + KEY_NOTIFY + KEY_CREATE_SUB_KEY + KEY_CREATE_LINK + KEY_SET_VALUE)�B */
/* KEY_READ �c �ǂݍ��݂����Ȃ�Ȃ�ł��ł��܂�(STANDARD_RIGHTS_READ + KEY_QUERY_VALUE + KEY_ENUMERATE_SUB_KEYS + KEY_NOTIFY)�B */
/* KEY_WRITE �c �������݂����Ȃ牽�ł��ł��܂�(STANDARD_RIGHTS_WRITE + KEY_SET_VALUE + KEY_CREATE_SUB_KEY)�B */

/* �@phkResult �c �����ɁA�J���ꂽ�L�[�ւ̃n���h�����Ԃ���܂��B */
  return self;
}

mrb_value
mrb_registry_create_key(mrb_state* mrb, mrb_value self)
{

/* LONG RegCreateKeyEx(HKEY hOpenKey, LPCTSTR lpSubKey, NULL, NULL, DWORD dwOptions, REGSAM samDesired, LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition ); */

/* �@hOpenKey �c ���łɊJ���ꂽ�L�[���w�肵�܂��B��������ɂ��āAlpSubKey���J���܂��B�n�C�u���w��ł��܂��B */
/* �@lpSubKey �c ���ꂩ��J�������T�u�L�[���w�肵�܂��B�擪�� \ �ł͂��߂Ă͂����܂���B */
/* �@dwOptions �c ���ꂩ��J�������L�[�ւ̃I�v�V�������w�肵�܂��B */

/* REG_OPTION_NON_VOLATILE(0) �c ���ʂ̕s�������̃L�[�B */
/* REG_OPTION_VOLATILE(1) �c �������̃L�[(Windows NT�n) */
/* REG_OPTION_BACKUP_RESTORE(4) �c samDesired�𖳎����ăo�b�N�A�b�v�p�̃L�[���쐬(Windows NT�n) */

/* �@samDesired �c �J�����L�[�ւ̃A�N�Z�X�������w�肵�܂��B */

/* KEY_CREATE_LINK(0x0020) �c �V���{���b�N�����N���쐬�ł��܂��B */
/* KEY_CREATE_SUB_KEY(0x0004) �c �T�u�L�[���쐬�ł��܂��B */
/* KEY_ENUMERATE_SUB_KEYS(0x0008) �c �T�u�L�[��񋓂ł��܂��B */
/* KEY_EXECUTE �c �ǂݎ��A�N�Z�X�ł��܂��B */
/* KEY_NOTIFY(0x0010) �c �ύX�̒ʒm�i�L�[�ɕύX��������ꂽ�Ƃ��ɁA���̂��Ƃ�ʒm����悤�v���j���ł��܂��B */
/* KEY_QUERY_VALUE(0x0001) �c �T�u�L�[�̃f�[�^��₢���킹�邱�Ƃ��ł��܂��B */
/* KEY_SET_VALUE(0x0002) �c �T�u�L�[�̃f�[�^��ݒ肷�邱�Ƃ��ł��܂��B */
/* KEY_ALL_ACCESS �c �Ȃ�ł��ł��܂�(KEY_QUERY_VALUE + KEY_ENUMERATE_SUB_KEYS + KEY_NOTIFY + KEY_CREATE_SUB_KEY + KEY_CREATE_LINK + KEY_SET_VALUE)�B */
/* KEY_READ �c �ǂݍ��݂����Ȃ�Ȃ�ł��ł��܂�(STANDARD_RIGHTS_READ + KEY_QUERY_VALUE + KEY_ENUMERATE_SUB_KEYS + KEY_NOTIFY)�B */
/* KEY_WRITE �c �������݂����Ȃ牽�ł��ł��܂�(STANDARD_RIGHTS_WRITE + KEY_SET_VALUE + KEY_CREATE_SUB_KEY)�B */

/* �@lpSecurityAttributes �c SECURITY_ATTRIBUTES�\���̂ւ̃|�C���^�BNULL�Ŋ���̃Z�L�����e�B�B */
/* �@phkResult �c �����ɁA�J���ꂽ�L�[�ւ̃n���h�����Ԃ���܂��B */
/* �@lpdwDisposition �c �V�����L�[���쐬���ꂽ�ꍇ��REG_CREATED_NEW_KEY(1)���A���łɂ���L�[���J���ꂽ�ꍇ��REG_OPENED_EXISTING_KEY(2)��Ԃ���܂��BNULL���w�肷��ƁA���ʂ͕Ԃ���܂���B */

/* �@ERROR_SUCCESS(0) �c ���� */
  return self;
}

mrb_value
mrb_registry_close_key(mrb_state* mrb, mrb_value self)
{
/* �@���W�X�g���L�[����܂��B */
/* LONG RegCloseKey( HKEY hOpenKey ); */
/* �@hOpenKey �c ���̃L�[����܂��B�n�C�u���w�肵�Ă͂����܂���B */

/* �@ERROR_SUCCESS(0) �c ���� */
  return self;
}

mrb_value
mrb_registry_query_value(mrb_state* mrb, mrb_value self)
{
/* �w�肳�ꂽ���W�X�g���l�́A�l�̌^�ƒl�̃f�[�^���擾���܂��B */
/* LONG RegQueryValueEx( HKEY hKey, LPCTSTR lpValueName, NULL, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData ); */
/* �@hKey �c ���W�X�g���L�[�ւ̃n���h�����w�肵�܂��B�n�C�u���w��ł��܂��B */
/* �@lpValueName �c �l�̖��O���w�肵�܂��B */
/* �@lpType �c�l�̌^���Ԃ���܂��B�^���擾���Ȃ��Ă��ǂ��Ƃ����ꍇ��NULL���w�肵�܂��B */
/* �@lpData �c �l�̃f�[�^���Ԃ���܂��B�\���ȃ���������������Ă��Ȃ���΂Ȃ�܂���B */
/* �@lpcbData �c lpData�̃T�C�Y���w�肵�܂��BlpData���\���ȃ������������Ă���̂��ǂ����𔻒f���邽�߂ɕK�v�ł��B������^�̏ꍇ�́A�I�[��\0������T�C�Y���Ȃ���΂Ȃ�܂���B�֐�����A��ƁAlpData�Ɋi�[���ꂽ�f�[�^�̃T�C�Y���������܂��B */

/* �@ERROR_SUCCESS(0) �c ���� */
/* �@�L�[���I�[�v������Ƃ��AKEY_QUERY_VALUE(0x0001)�������Ȃ���΂Ȃ�܂���B */
/* �@Windows 95/98/Me�ł́A���W�X�g���̒l�̖��O��255�����܂łł��B */
  return self;
}

mrb_value
mrb_registry_set_value(mrb_state* mrb, mrb_value self)
{
/* �w�肳�ꂽ���W�X�g���̒l�Ƀf�[�^���������݂܂��B */
/* LONG RegSetValueEx( HKEY hKey, LPCTSTR lpValueName, NULL, DWORD dwType, CONST BYTE *lpData, DWORD cbData ); */
/* �@hKey �c ���W�X�g���L�[�ւ̃n���h�����w�肵�܂��B�n�C�u���w��ł��܂��B */
/* �@lpValueName �c �l�̖��O���w�肵�܂��B */
/* �@dwType �c�l�̌^���w�肵�܂��B */
/* �@lpData �c �l�̃f�[�^�ւ̃|�C���^���w�肵�܂��B */
/* �@cbData �c lpData�̃T�C�Y���w�肵�܂��B������^�̏ꍇ�́A�I�[��\0���܂񂾃T�C�Y���w�肵�܂��B */

/* �@ERROR_SUCCESS(0) �c ���� */
/* �@�L�[���I�[�v������Ƃ��AKEY_SET_VALUE(0x0002)�������Ȃ���΂Ȃ�܂���B */
  return self;
}

mrb_value
mrb_registry_delete_key(mrb_state* mrb, mrb_value self)
{
/* �@���W�X�g���L�[���폜���܂��B�L�[�̒��̃G���g���͂��ׂč폜����܂��B */
/* LONG RegDeleteKey( HKEY hKey, LPCTSTR lpSubKey ); */
/* �@hKey �c ���W�X�g���L�[�ւ̃n���h�����w�肵�܂��B�n�C�u���w��ł��܂��B */
/* �@lpSubKey �c �폜�������T�u�L�[�̖��O���w�肵�܂��B */

/* �@ERROR_SUCCESS(0) �c ���� */
  return self;
}

mrb_value
mrb_registry_delete_value(mrb_state* mrb, mrb_value self)
{
/* ���W�X�g���L�[���폜���܂��B�L�[�̒��̃G���g���͂��ׂč폜����܂��B */
/* LONG RegDeleteValue( HKEY hKey, LPCTSTR lpValueName ); */
/* �@hKey �c ���W�X�g���L�[�ւ̃n���h�����w�肵�܂��B�n�C�u���w��ł��܂��B */
/* �@lpValueName �c �l�̖��O���w�肵�܂��B */

/* �@ERROR_SUCCESS(0) �c ���� */
/* �@hkey��KEY_SET_VALUE(0x0002)�������K�v�ł��B */
  return self;
}

mrb_value
mrb_registry_enum_key(mrb_state* mrb, mrb_value self)
{
/* �L�[���̃T�u�L�[��񋓂��܂��B */
/* LONG RegEnumKeyEx( HKEY hKey, DWORD dwIndex, LPTSTR lpName, LPDWORD lpcName, NULL, NULL, NULL, PFILETIME lpftLastWriteTime ); */
/* �@hKey �c ���W�X�g���L�[�ւ̃n���h�����w�肵�܂��B�n�C�u���w��ł��܂��B */
/* �@dwIndex �c �ԍ����w�肵�܂��B������0,1,2,�c�Ə��ԂɎw�肵�Ă������Ƃɂ���āA�T�u�L�[�����X��񋓂��Ă����܂��B */
/* �@lpName �c �T�u�L�[�̖��O���Ԃ����o�b�t�@�ւ̃|�C���^�B */
/* �@lpcName �c DWORD�^�ϐ��ւ̃|�C���^�B�֐����Ăяo���Ƃ��́A�ϐ��ɂ�lpName�̎w���o�b�t�@�̃T�C�Y(�I�[��\0���܂�)�����Ă����܂��B�֐�����������ƁA���ۂ�lpName�ɕԂ��ꂽ���O�̒���(�I�[��\0������)���������Ă��܂��B */
/* �@lpftLastWriteTime �c �T�u�L�[�̍ŏI�������ݓ������������܂��FILETIME�^�ϐ��ւ̃|�C���^�ł��B */

/* �@ERROR_SUCCESS(0) �c ���� */
/* �@ERROR_NO_MORE_ITEMS(259) �c �񋓏I�� */

/* �@�񋓂��Ȃ���L�[���폜������쐬�����肵�Ă͂����܂���B */
/* �@���̊֐����g���ɂ́AhKey��KEY_ENUMERATE_SUB_KEYS(0x0008)�������Ȃ���΂Ȃ�܂���B */
  return self;
}

mrb_value
mrb_registry_enum_value(mrb_state* mrb, mrb_value self)
{
/* �L�[�̒��̒l��񋓂��܂��B */
/* LONG RegEnumValue( HKEY hKey, DWORD dwIndex, LPTSTR lpValueName, LPDWORD lpcValueName, NULL, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData ); */
/* �@hKey �c ���W�X�g���L�[�ւ̃n���h�����w�肵�܂��B�n�C�u���w��ł��܂��B */
/* �@dwIndex �c �ԍ����w�肵�܂��B������0,1,2,�c�Ə��ԂɎw�肵�Ă������Ƃɂ���āA�l�����X��񋓂��Ă����܂��B */
/* �@lpValueName �c �l�̖��O���Ԃ����o�b�t�@�ւ̃|�C���^�B */
/* �@lpcValueName �c DWORD�^�ϐ��ւ̃|�C���^�B�֐����Ăяo���Ƃ��́A�ϐ��ɂ�lpValueName�̎w���o�b�t�@�̃T�C�Y(�I�[��\0���܂�)�����Ă����܂��B�֐�����������ƁA���ۂ�lpValueName�ɕԂ��ꂽ���O�̒���(�I�[��\0������)���������Ă��܂��B */
/* �@lpType �c �l�̌^���Ԃ���܂��B�^���擾���Ȃ��Ă��ǂ��Ƃ����ꍇ��NULL���w�肵�܂��B */
/* �@lpData �c �l�̃f�[�^���Ԃ���܂��B�\���ȃ���������������Ă��Ȃ���΂Ȃ�܂���B���̏�񂪕s�v�ȏꍇ�́ANULL���w��ł��܂��B */
/* �@lpcbData �c lpData�̃T�C�Y���w�肵�܂��BlpData���\���ȃ������������Ă���̂��ǂ����𔻒f���邽�߂ɕK�v�ł��B������^�̏ꍇ�́A�I�[��\0������T�C�Y���Ȃ���΂Ȃ�܂���B�֐�����A��ƁAlpData�Ɋi�[���ꂽ�f�[�^�̃T�C�Y���������܂��BlpData��NULL�̏ꍇ�́A�����ɂ�NULL���w�肵�܂��B */

/* �@ERROR_SUCCESS(0) �c ���� */
/* �@ERROR_NO_MORE_ITEMS(259) �c �񋓏I�� */

/* �@�񋓂��Ȃ���l���폜������쐬���Ă͂����܂���B */
/* �@���̊֐����g���ɂ́AhKey��KEY_QUERY_VALUE(0x0001)�������Ȃ���΂Ȃ�܂���B */
  return self;
}

mrb_value
mrb_registry_query_info_key(mrb_state* mrb, mrb_value self)
{
/* �w�肳�ꂽ���W�X�g���L�[�Ɋւ�������擾���܂��B */
/* LONG RegQueryInfoKey( HKEY hKey, NULL, NULL, NULL, LPDWORD lpcSubKeys, LPDWORD lpcMaxSubKeyLen, NULL, LPDWORD lpcValues, LPDWORD lpcMaxValueNameLen, LPDWORD lpcMaxValueLen, LPDWORD lpcbSecurityDescriptor, PFILETIME lpftLastWriteTime ); */
/* �@hKey �c ���W�X�g���L�[�ւ̃n���h�����w�肵�܂��B�n�C�u���w��ł��܂��B */
/* �@lpcSubKeys �c hKey�̒��Ɋ܂܂��T�u�L�[�̐����擾���܂��B�s�v�ȏꍇ��NULL���w�肵�܂��B */
/* �@lpcMaxSubKeyLen �c hKey�̒��Ɋ܂܂��T�u�L�[�̒��ŁA�ł����O�̒����L�[�́A���O�̒������擾���܂��B�s�v�ȏꍇ��NULL���w��ł��܂��B�Ȃ��AWindows 95/98/Me�ł́A������̒����ɏI�[��\0���܂݂܂����AWindows NT�n�ł́A������̒����ɏI�[��\0���܂݂܂���B */
/* �@lpcValues �c hKey�̒��Ɋ܂܂��l�̐����擾���܂��B�s�v�ȏꍇ��NULL���w�肵�܂��B */
/* �@lpcMaxValueNameLen �c hKey�̒��Ɋ܂܂��l�̒��ŁA�ł����O�̒����l�́A���O�̒������擾���܂��B�s�v�ȏꍇ��NULL���w��ł��܂��B */
/* �@lpcbSecurityDescriptor �c �L�[�̃Z�L�����e�B�L�q�q�̒������o�C�g�P�ʂŊi�[����܂��B�s�v�ȏꍇ��NULL���w��ł��܂��B */
/* �@lpftLastWriteTime �c �L�[�̍ŏI�������ݓ����𓾂܂��B�s�v�ȏꍇ��NULL���w��ł��܂��B�܂��AWindows 95/98/Me�ł͍ŏI�������ݓ����������܂���̂ŁA�K��NULL���w�肵�܂��B */

/* �@ERROR_SUCCESS(0) �c ���� */

/* �@���̊֐����g���ɂ́AhKey��KEY_QUERY_VALUE(0x0001)�������Ȃ���΂Ȃ�܂���B */
  return self;
}

void
mrb_mruby_winapi_registry_gem_init(mrb_state* mrb)
{
    struct RClass* winapi;
    struct RClass* regkey;
    struct RClass* regval;
    struct RClass* reg_utils;

    winapi = mrb_module_get(mrb, "WinAPI");
    reg = mrb_define_module_under(mrb, winapi, "Registry", mrb->object_class);
    regkey = mrb_define_class_under(mrb, reg, "Key", mrb->object_class);
    regval = mrb_define_class_under(mrb, reg, "Value", mrb->object_class);

    /* Dir chdir, chroot, delete, rmdir, unlink, entries, exist? exsits? pwd, mkdir, new, open */

    /* mrb_registry_open_key; */
    /* mrb_registry_create_key; */
    /* mrb_registry_close_key; */
    /* mrb_registry_query_value; */
    /* mrb_registry_set_value; */
    /* mrb_registry_delete_key; */ /* RegUtils.rm_rf */
    /* mrb_registry_delete_value; */
    /* mrb_registry_enum_key; */
    /* mrb_registry_enum_value; */
    /* mrb_registry_query_info_key; */

/* RegConnectRegistry */
/* RegFlushKey */
/* RegLoadKey */
/* RegNotifyChangeKeyValue */
/* RegOpenCurrentUser */
/* RegOpenUserClassesRoot */
/* RegOverridePredefKey */
/* RegQueryMultipleValues */
/* RegReplaceKey */
/* RegRestoreKey */
/* RegSaveKey */
/* RegUnLoadKey */
}

void
mrb_mruby_winapi_registry_gem_final(mrb_state* mrb)
{
}

