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
/* アプリケーションの関連づけ・CLSID管理。 */
/* HKEY_CURRENT_USER (0x80000001) */
/* ログオンユーザのプロファイル。 */
/* HKEY_LOCAL_MACHINE (0x80000002) */
/* システム全体の設定。 HARDWAREサブキーの下はシステム起動時に採集される。 */
/* HKEY_USERS (0x80000003) */
/* ユーザ全体のプロファイル。 ただし、通常ログオン中ユーザのものとデフォルトのみがロードされている。 */
/* HKEY_PERFORMANCE_DATA (0x80000004) */
/* NTパフォーマンスデータ。 API呼び出し毎にパフォーマンス取得用DLLにより採集される。 */
/* HKEY_CURRENT_CONFIG (0x80000005) */
/* 現在のシステム設定。 */
/* HKEY_DYN_DATA (0x80000006) */
/* 95リソースなどの変動データ。 */

mrb_value
mrb_registry_open_key(mrb_state* mrb, mrb_value self)
{

/* ONG RegOpenKeyEx(HKEY hOpenKey, LPCTSTR lpSubKey, NULL, REGSAM samDesired, PHKEY phkResult); */
/* 　hOpenKey … すでに開かれたキーを指定します。ここを基準にして、lpSubKeyを開きます。ハイブを指定できます。 */
/* 　lpSubKey … これから開きたいサブキーを指定します。先頭を \ ではじめてはいけません。 */
/* 　samDesired … 開かれるキーへのアクセス権限を指定します。 */

/* KEY_CREATE_LINK(0x0020) … シンボリックリンクを作成できます。 */
/* KEY_CREATE_SUB_KEY(0x0004) … サブキーを作成できます。 */
/* KEY_ENUMERATE_SUB_KEYS(0x0008) … サブキーを列挙できます。 */
/* KEY_EXECUTE … 読み取りアクセスできます。 */
/* KEY_NOTIFY(0x0010) … 変更の通知（キーに変更が加えられたときに、そのことを通知するよう要求）をできます。 */
/* KEY_QUERY_VALUE(0x0001) … サブキーのデータを問い合わせることができます。 */
/* KEY_SET_VALUE(0x0002) … サブキーのデータを設定することができます。 */
/* KEY_ALL_ACCESS … なんでもできます(KEY_QUERY_VALUE + KEY_ENUMERATE_SUB_KEYS + KEY_NOTIFY + KEY_CREATE_SUB_KEY + KEY_CREATE_LINK + KEY_SET_VALUE)。 */
/* KEY_READ … 読み込みだけならなんでもできます(STANDARD_RIGHTS_READ + KEY_QUERY_VALUE + KEY_ENUMERATE_SUB_KEYS + KEY_NOTIFY)。 */
/* KEY_WRITE … 書き込みだけなら何でもできます(STANDARD_RIGHTS_WRITE + KEY_SET_VALUE + KEY_CREATE_SUB_KEY)。 */

/* 　phkResult … ここに、開かれたキーへのハンドルが返されます。 */
  return self;
}

mrb_value
mrb_registry_create_key(mrb_state* mrb, mrb_value self)
{

/* LONG RegCreateKeyEx(HKEY hOpenKey, LPCTSTR lpSubKey, NULL, NULL, DWORD dwOptions, REGSAM samDesired, LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition ); */

/* 　hOpenKey … すでに開かれたキーを指定します。ここを基準にして、lpSubKeyを開きます。ハイブを指定できます。 */
/* 　lpSubKey … これから開きたいサブキーを指定します。先頭を \ ではじめてはいけません。 */
/* 　dwOptions … これから開きたいキーへのオプションを指定します。 */

/* REG_OPTION_NON_VOLATILE(0) … 普通の不揮発性のキー。 */
/* REG_OPTION_VOLATILE(1) … 揮発性のキー(Windows NT系) */
/* REG_OPTION_BACKUP_RESTORE(4) … samDesiredを無視してバックアップ用のキーを作成(Windows NT系) */

/* 　samDesired … 開かれるキーへのアクセス権限を指定します。 */

/* KEY_CREATE_LINK(0x0020) … シンボリックリンクを作成できます。 */
/* KEY_CREATE_SUB_KEY(0x0004) … サブキーを作成できます。 */
/* KEY_ENUMERATE_SUB_KEYS(0x0008) … サブキーを列挙できます。 */
/* KEY_EXECUTE … 読み取りアクセスできます。 */
/* KEY_NOTIFY(0x0010) … 変更の通知（キーに変更が加えられたときに、そのことを通知するよう要求）をできます。 */
/* KEY_QUERY_VALUE(0x0001) … サブキーのデータを問い合わせることができます。 */
/* KEY_SET_VALUE(0x0002) … サブキーのデータを設定することができます。 */
/* KEY_ALL_ACCESS … なんでもできます(KEY_QUERY_VALUE + KEY_ENUMERATE_SUB_KEYS + KEY_NOTIFY + KEY_CREATE_SUB_KEY + KEY_CREATE_LINK + KEY_SET_VALUE)。 */
/* KEY_READ … 読み込みだけならなんでもできます(STANDARD_RIGHTS_READ + KEY_QUERY_VALUE + KEY_ENUMERATE_SUB_KEYS + KEY_NOTIFY)。 */
/* KEY_WRITE … 書き込みだけなら何でもできます(STANDARD_RIGHTS_WRITE + KEY_SET_VALUE + KEY_CREATE_SUB_KEY)。 */

/* 　lpSecurityAttributes … SECURITY_ATTRIBUTES構造体へのポインタ。NULLで既定のセキュリティ。 */
/* 　phkResult … ここに、開かれたキーへのハンドルが返されます。 */
/* 　lpdwDisposition … 新しくキーが作成された場合はREG_CREATED_NEW_KEY(1)を、すでにあるキーが開かれた場合はREG_OPENED_EXISTING_KEY(2)を返されます。NULLを指定すると、結果は返されません。 */

/* 　ERROR_SUCCESS(0) … 成功 */
  return self;
}

mrb_value
mrb_registry_close_key(mrb_state* mrb, mrb_value self)
{
/* 　レジストリキーを閉じます。 */
/* LONG RegCloseKey( HKEY hOpenKey ); */
/* 　hOpenKey … このキーを閉じます。ハイブを指定してはいけません。 */

/* 　ERROR_SUCCESS(0) … 成功 */
  return self;
}

mrb_value
mrb_registry_query_value(mrb_state* mrb, mrb_value self)
{
/* 指定されたレジストリ値の、値の型と値のデータを取得します。 */
/* LONG RegQueryValueEx( HKEY hKey, LPCTSTR lpValueName, NULL, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData ); */
/* 　hKey … レジストリキーへのハンドルを指定します。ハイブを指定できます。 */
/* 　lpValueName … 値の名前を指定します。 */
/* 　lpType …値の型が返されます。型を取得しなくても良いという場合はNULLを指定します。 */
/* 　lpData … 値のデータが返されます。十分なメモリが準備されていなければなりません。 */
/* 　lpcbData … lpDataのサイズを指定します。lpDataが十分なメモリを持っているのかどうかを判断するために必要です。文字列型の場合は、終端の\0を入れるサイズがなければなりません。関数から帰ると、lpDataに格納されたデータのサイズが代入されます。 */

/* 　ERROR_SUCCESS(0) … 成功 */
/* 　キーをオープンするとき、KEY_QUERY_VALUE(0x0001)権限がなければなりません。 */
/* 　Windows 95/98/Meでは、レジストリの値の名前は255文字までです。 */
  return self;
}

mrb_value
mrb_registry_set_value(mrb_state* mrb, mrb_value self)
{
/* 指定されたレジストリの値にデータを書き込みます。 */
/* LONG RegSetValueEx( HKEY hKey, LPCTSTR lpValueName, NULL, DWORD dwType, CONST BYTE *lpData, DWORD cbData ); */
/* 　hKey … レジストリキーへのハンドルを指定します。ハイブを指定できます。 */
/* 　lpValueName … 値の名前を指定します。 */
/* 　dwType …値の型を指定します。 */
/* 　lpData … 値のデータへのポインタを指定します。 */
/* 　cbData … lpDataのサイズを指定します。文字列型の場合は、終端の\0も含んだサイズを指定します。 */

/* 　ERROR_SUCCESS(0) … 成功 */
/* 　キーをオープンするとき、KEY_SET_VALUE(0x0002)権限がなければなりません。 */
  return self;
}

mrb_value
mrb_registry_delete_key(mrb_state* mrb, mrb_value self)
{
/* 　レジストリキーを削除します。キーの中のエントリはすべて削除されます。 */
/* LONG RegDeleteKey( HKEY hKey, LPCTSTR lpSubKey ); */
/* 　hKey … レジストリキーへのハンドルを指定します。ハイブを指定できます。 */
/* 　lpSubKey … 削除したいサブキーの名前を指定します。 */

/* 　ERROR_SUCCESS(0) … 成功 */
  return self;
}

mrb_value
mrb_registry_delete_value(mrb_state* mrb, mrb_value self)
{
/* レジストリキーを削除します。キーの中のエントリはすべて削除されます。 */
/* LONG RegDeleteValue( HKEY hKey, LPCTSTR lpValueName ); */
/* 　hKey … レジストリキーへのハンドルを指定します。ハイブを指定できます。 */
/* 　lpValueName … 値の名前を指定します。 */

/* 　ERROR_SUCCESS(0) … 成功 */
/* 　hkeyはKEY_SET_VALUE(0x0002)権限が必要です。 */
  return self;
}

mrb_value
mrb_registry_enum_key(mrb_state* mrb, mrb_value self)
{
/* キー内のサブキーを列挙します。 */
/* LONG RegEnumKeyEx( HKEY hKey, DWORD dwIndex, LPTSTR lpName, LPDWORD lpcName, NULL, NULL, NULL, PFILETIME lpftLastWriteTime ); */
/* 　hKey … レジストリキーへのハンドルを指定します。ハイブを指定できます。 */
/* 　dwIndex … 番号を指定します。ここを0,1,2,…と順番に指定していくことによって、サブキーを次々を列挙していきます。 */
/* 　lpName … サブキーの名前が返されるバッファへのポインタ。 */
/* 　lpcName … DWORD型変数へのポインタ。関数を呼び出すときは、変数にはlpNameの指すバッファのサイズ(終端の\0も含む)を入れておきます。関数が成功すると、実際にlpNameに返された名前の長さ(終端の\0を除く)が代入されてきます。 */
/* 　lpftLastWriteTime … サブキーの最終書き込み日時が書き込まれるFILETIME型変数へのポインタです。 */

/* 　ERROR_SUCCESS(0) … 成功 */
/* 　ERROR_NO_MORE_ITEMS(259) … 列挙終了 */

/* 　列挙しながらキーを削除したり作成したりしてはいけません。 */
/* 　この関数を使うには、hKeyにKEY_ENUMERATE_SUB_KEYS(0x0008)権限がなければなりません。 */
  return self;
}

mrb_value
mrb_registry_enum_value(mrb_state* mrb, mrb_value self)
{
/* キーの中の値を列挙します。 */
/* LONG RegEnumValue( HKEY hKey, DWORD dwIndex, LPTSTR lpValueName, LPDWORD lpcValueName, NULL, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData ); */
/* 　hKey … レジストリキーへのハンドルを指定します。ハイブを指定できます。 */
/* 　dwIndex … 番号を指定します。ここを0,1,2,…と順番に指定していくことによって、値を次々を列挙していきます。 */
/* 　lpValueName … 値の名前が返されるバッファへのポインタ。 */
/* 　lpcValueName … DWORD型変数へのポインタ。関数を呼び出すときは、変数にはlpValueNameの指すバッファのサイズ(終端の\0も含む)を入れておきます。関数が成功すると、実際にlpValueNameに返された名前の長さ(終端の\0を除く)が代入されてきます。 */
/* 　lpType … 値の型が返されます。型を取得しなくても良いという場合はNULLを指定します。 */
/* 　lpData … 値のデータが返されます。十分なメモリが準備されていなければなりません。この情報が不要な場合は、NULLを指定できます。 */
/* 　lpcbData … lpDataのサイズを指定します。lpDataが十分なメモリを持っているのかどうかを判断するために必要です。文字列型の場合は、終端の\0を入れるサイズがなければなりません。関数から帰ると、lpDataに格納されたデータのサイズが代入されます。lpDataがNULLの場合は、ここにもNULLを指定します。 */

/* 　ERROR_SUCCESS(0) … 成功 */
/* 　ERROR_NO_MORE_ITEMS(259) … 列挙終了 */

/* 　列挙しながら値を削除したり作成してはいけません。 */
/* 　この関数を使うには、hKeyにKEY_QUERY_VALUE(0x0001)権限がなければなりません。 */
  return self;
}

mrb_value
mrb_registry_query_info_key(mrb_state* mrb, mrb_value self)
{
/* 指定されたレジストリキーに関する情報を取得します。 */
/* LONG RegQueryInfoKey( HKEY hKey, NULL, NULL, NULL, LPDWORD lpcSubKeys, LPDWORD lpcMaxSubKeyLen, NULL, LPDWORD lpcValues, LPDWORD lpcMaxValueNameLen, LPDWORD lpcMaxValueLen, LPDWORD lpcbSecurityDescriptor, PFILETIME lpftLastWriteTime ); */
/* 　hKey … レジストリキーへのハンドルを指定します。ハイブを指定できます。 */
/* 　lpcSubKeys … hKeyの中に含まれるサブキーの数を取得します。不要な場合はNULLを指定します。 */
/* 　lpcMaxSubKeyLen … hKeyの中に含まれるサブキーの中で、最も名前の長いキーの、名前の長さを取得します。不要な場合はNULLを指定できます。なお、Windows 95/98/Meでは、文字列の長さに終端の\0を含みますが、Windows NT系では、文字列の長さに終端の\0を含みません。 */
/* 　lpcValues … hKeyの中に含まれる値の数を取得します。不要な場合はNULLを指定します。 */
/* 　lpcMaxValueNameLen … hKeyの中に含まれる値の中で、最も名前の長い値の、名前の長さを取得します。不要な場合はNULLを指定できます。 */
/* 　lpcbSecurityDescriptor … キーのセキュリティ記述子の長さがバイト単位で格納されます。不要な場合はNULLを指定できます。 */
/* 　lpftLastWriteTime … キーの最終書き込み日時を得ます。不要な場合はNULLを指定できます。また、Windows 95/98/Meでは最終書き込み日時を扱えませんので、必ずNULLを指定します。 */

/* 　ERROR_SUCCESS(0) … 成功 */

/* 　この関数を使うには、hKeyにKEY_QUERY_VALUE(0x0001)権限がなければなりません。 */
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

