#include "keyboard.h"
#include <mruby/hash.h>

struct mrb_keyboard*
mrb_keyboard_malloc(mrb_state *mrb)
{
    struct mrb_keyboard *keyboard;
    keyboard = (struct mrb_keyboard *)mrb_malloc(mrb, sizeof(*keyboard));
    ZeroMemory(&(keyboard->ki), sizeof(keyboard->ki));
    return keyboard;
}

void
mrb_keyboard_free(mrb_state *mrb, void *ptr)
{
    mrb_free(mrb, ptr);
}

// ----------------------
// construction
mrb_value
mrb_keyboard_new(mrb_state* mrb, struct mrb_keyboard* keyboard)
{
    return mrb_obj_value(Data_Wrap_Struct(mrb, mrb_class_get(mrb, "Keyboard"), &mrb_keyboard_type, keyboard));
}

mrb_value
mrb_keyboard_new_alloc(mrb_state* mrb, mrb_value self)
{
    return mrb_keyboard_new(mrb, mrb_keyboard_malloc(mrb));
}


mrb_value
mrb_keyboard_send(mrb_state* mrb, mrb_value self)
{
  int size;
  mrb_value* key;
  mrb_get_args(mrb, "*", &key, &size);
  {
    INPUT inputs[size];
    {    
      int i;
      for(i=0; i<size; ++i) {
        INPUT input;
        KEYBDINPUT ki;
        
        input.type = INPUT_KEYBOARD;
        ZeroMemory(&ki, sizeof(ki));
        ki.wVk = KEYBOARD(key[i])->ki.wVk;
        ki.wScan = KEYBOARD(key[i])->ki.wScan;
        ki.dwFlags = KEYBOARD(key[i])->ki.dwFlags;
        input.ki = ki;
        inputs[i] = input;
        /* fprintf(stdout, "key send: %x, %x, %x\n", ki.wVk, ki.wScan, ki.dwFlags); */
      }
    }
    /* fprintf(stdout, "%d keys send\n", size); */
    SendInput(size,inputs,sizeof(INPUT));
  }
  return mrb_nil_value();
}

mrb_value
mrb_keyboard_set_extend(mrb_state* mrb, mrb_value self)
{
    KEYBOARD(self)->ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
    return self;
}

mrb_value
mrb_keyboard_unset_extend(mrb_state* mrb, mrb_value self)
{
    KEYBOARD(self)->ki.dwFlags &= ~KEYEVENTF_EXTENDEDKEY;
    return self;
}

mrb_value
mrb_keyboard_set_unicode(mrb_state* mrb, mrb_value self)
{
    KEYBOARD(self)->ki.dwFlags |= KEYEVENTF_UNICODE;
    return self;
}

mrb_value
mrb_keyboard_unset_unicode(mrb_state* mrb, mrb_value self)
{
    KEYBOARD(self)->ki.dwFlags &= ~KEYEVENTF_UNICODE;
    return self;
}

mrb_value
mrb_keyboard_set_scancode(mrb_state* mrb, mrb_value self)
{
    KEYBOARD(self)->ki.dwFlags |= KEYEVENTF_SCANCODE;
    return self;
}

mrb_value
mrb_keyboard_unset_scancode(mrb_state* mrb, mrb_value self)
{
    KEYBOARD(self)->ki.dwFlags &= ~KEYEVENTF_SCANCODE;
    return self;
}

mrb_value
mrb_keyboard_up(mrb_state* mrb, mrb_value self)
{
    KEYBOARD(self)->ki.dwFlags |= KEYEVENTF_KEYUP;
    return self;
}

mrb_value
mrb_keyboard_down(mrb_state* mrb, mrb_value self)
{
    KEYBOARD(self)->ki.dwFlags &= ~KEYEVENTF_KEYUP;
    return self;
}

mrb_value
mrb_keyboard_set_virtual(mrb_state* mrb, mrb_value self)
{
    mrb_value vk;
    mrb_get_args(mrb, "i", &vk);
    KEYBOARD(self)->ki.wVk = mrb_fixnum(vk);
    if (!KEYBOARD(self)->ki.wScan) {
        KEYBOARD(self)->ki.wScan = MapVirtualKey( KEYBOARD(self)->ki.wVk, 0 );
    }
    return self;
}

mrb_value
mrb_keyboard_set_scan(mrb_state* mrb, mrb_value self)
{
    mrb_value scan;
    mrb_get_args(mrb, "i", &scan);
    KEYBOARD(self)->ki.wScan = mrb_fixnum(scan);
    return self;
}

mrb_value
mrb_keyboard_set_flags(mrb_state* mrb, mrb_value self)
{
    mrb_value flags;
    mrb_get_args(mrb, "i", &flags);
    KEYBOARD(self)->ki.dwFlags = mrb_fixnum(flags);
    return self;
}

mrb_value
mrb_keyboard_get_virtual(mrb_state* mrb, mrb_value self)
{
    return mrb_fixnum_value(KEYBOARD(self)->ki.wVk);
}

mrb_value
mrb_keyboard_get_scan(mrb_state* mrb, mrb_value self)
{
    return mrb_fixnum_value(KEYBOARD(self)->ki.wScan);
}

mrb_value
mrb_keyboard_to_scan(mrb_state* mrb, mrb_value self)
{
    mrb_value vk;
    mrb_value mode;
    mrb_get_args(mrb, "i|i", &vk, &mode);
    if(mrb_fixnum_p(mode)) {
        return mrb_fixnum_value(MapVirtualKey(mrb_fixnum(vk), mrb_fixnum(mode)));
    } else {
        /* fprintf(stdout, "%x, %x\n", mrb_fixnum(vk), MapVirtualKey(mrb_fixnum(vk), 0)); */
        return mrb_fixnum_value(MapVirtualKey(mrb_fixnum(vk), 0));
    }
}

void
mrb_mruby_winapi_keyboard_gem_init(mrb_state* mrb)
{
    struct RClass* kbd;

    kbd = mrb_define_class(mrb, "Keyboard", mrb->object_class);
    MRB_SET_INSTANCE_TT(kbd, MRB_TT_DATA);
    mrb_define_class_method(mrb, kbd, "new", mrb_keyboard_new_alloc, ARGS_NONE());
    mrb_define_class_method(mrb, kbd, "send", mrb_keyboard_send, ARGS_REST());

    mrb_define_class_method(mrb, kbd, "to_scan", mrb_keyboard_to_scan, ARGS_REQ(1)|ARGS_OPT(1));
    
    mrb_define_method(mrb, kbd, "key=", mrb_keyboard_set_virtual, ARGS_REQ(1));
    mrb_define_method(mrb, kbd, "key", mrb_keyboard_get_virtual, ARGS_NONE());
    mrb_define_method(mrb, kbd, "scan=", mrb_keyboard_set_scan, ARGS_REQ(1));
    mrb_define_method(mrb, kbd, "scan", mrb_keyboard_get_scan, ARGS_NONE());
    mrb_define_method(mrb, kbd, "flag=", mrb_keyboard_set_flags, ARGS_REQ(1));
    
    mrb_define_method(mrb, kbd, "noextend", mrb_keyboard_unset_extend, ARGS_NONE());
    mrb_define_method(mrb, kbd, "extend", mrb_keyboard_set_extend, ARGS_NONE());
    mrb_define_method(mrb, kbd, "nounicode", mrb_keyboard_unset_unicode, ARGS_NONE());
    mrb_define_method(mrb, kbd, "unicode", mrb_keyboard_set_unicode, ARGS_NONE());
    mrb_define_method(mrb, kbd, "noscancode", mrb_keyboard_unset_scancode, ARGS_NONE());
    mrb_define_method(mrb, kbd, "scancode", mrb_keyboard_set_scancode, ARGS_NONE());
    mrb_define_method(mrb, kbd, "up", mrb_keyboard_up, ARGS_NONE());
    mrb_define_method(mrb, kbd, "down", mrb_keyboard_down, ARGS_NONE());


    {
          mrb_value scan;

          scan = mrb_hash_new(mrb);
          mrb_define_const(mrb, kbd, "Scan", scan);

          mrb_hash_set(mrb, scan, mrb_nil_value(), mrb_fixnum_value(0x00));
    
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "ESC"), mrb_fixnum_value(0x01));
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "F1"), mrb_fixnum_value(0x3B)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "F2"), mrb_fixnum_value(0x3C)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "F3"), mrb_fixnum_value(0x3D)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "F4"), mrb_fixnum_value(0x3E)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "F5"), mrb_fixnum_value(0x3F)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "F6"), mrb_fixnum_value(0x40)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "F7"), mrb_fixnum_value(0x41)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "F8"), mrb_fixnum_value(0x42)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "F9"), mrb_fixnum_value(0x43)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "F10"), mrb_fixnum_value(0x44)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "F11"), mrb_fixnum_value(0x57));
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "F12"), mrb_fixnum_value(0x58));
          /* mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "BREAK"), mrb_fixnum_value(0x45));  */
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "PAUSE"), mrb_fixnum_value(0x45));
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "PRINTSCREEN"), mrb_fixnum_value(0x37)); /* SYSRQ + EXT */
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "DELETE"), mrb_fixnum_value(0x53)); /* NUMPERIOD + EXT */
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "SYSRQ"), mrb_fixnum_value(0x37));

          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "HANKAKU_ZENKAKU"), mrb_fixnum_value(0x29)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "ONE"), mrb_fixnum_value(0x02)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "TWO"), mrb_fixnum_value(0x03)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "THREE"), mrb_fixnum_value(0x04)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "FOUR"), mrb_fixnum_value(0x05)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "FIVE"), mrb_fixnum_value(0x06)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "SIX"), mrb_fixnum_value(0x07)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "SEVEN"), mrb_fixnum_value(0x08)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "EIGHT"), mrb_fixnum_value(0x09)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "NINE"), mrb_fixnum_value(0x0A)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "ZERO"), mrb_fixnum_value(0x0B)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "MINUS"), mrb_fixnum_value(0x0C)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "CARET"), mrb_fixnum_value(0x0D)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "YEN"), mrb_fixnum_value(0x7D)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "BS"), mrb_fixnum_value(0x0E)); 

          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "TAB"), mrb_fixnum_value(0x0F)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "Q"), mrb_fixnum_value(0x10)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "W"), mrb_fixnum_value(0x11)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "E"), mrb_fixnum_value(0x12)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "R"), mrb_fixnum_value(0x13)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "T"), mrb_fixnum_value(0x14)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "Y"), mrb_fixnum_value(0x15)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "U"), mrb_fixnum_value(0x16)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "I"), mrb_fixnum_value(0x17)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "O"), mrb_fixnum_value(0x18)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "P"), mrb_fixnum_value(0x19)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "AT"), mrb_fixnum_value(0x1A)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "LBRACKET"), mrb_fixnum_value(0x1B)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "ENTER"), mrb_fixnum_value(0x1C)); 

          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "CAPSLOCK"), mrb_fixnum_value(0x3A)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "A"), mrb_fixnum_value(0x1E)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "S"), mrb_fixnum_value(0x1F)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "D"), mrb_fixnum_value(0x20)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "F"), mrb_fixnum_value(0x21)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "G"), mrb_fixnum_value(0x22)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "H"), mrb_fixnum_value(0x23)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "J"), mrb_fixnum_value(0x24)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "K"), mrb_fixnum_value(0x25)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "L"), mrb_fixnum_value(0x26)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "SEMICOLON"), mrb_fixnum_value(0x27)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "COLON"), mrb_fixnum_value(0x28)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "RBRACKET"), mrb_fixnum_value(0x2B)); 

          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "LSHIFT"), mrb_fixnum_value(0x2A)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "Z"), mrb_fixnum_value(0x2C)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "X"), mrb_fixnum_value(0x2D)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "C"), mrb_fixnum_value(0x2E)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "V"), mrb_fixnum_value(0x2F)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "B"), mrb_fixnum_value(0x30)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "N"), mrb_fixnum_value(0x31)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "M"), mrb_fixnum_value(0x32)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "COMMA"), mrb_fixnum_value(0x33)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "PERIOD"), mrb_fixnum_value(0x34)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "SLASH"), mrb_fixnum_value(0x35));
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "BACKSLASH"), mrb_fixnum_value(0x73)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "RSHIFT"), mrb_fixnum_value(0x36));
    
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "LCONTROL"), mrb_fixnum_value(0x1D)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "LWIN"), mrb_fixnum_value(0x5B)); /* X + EXT */
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "LALT"), mrb_fixnum_value(0x38));
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "NONCONVERT"), mrb_fixnum_value(0x7B)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "SPACE"), mrb_fixnum_value(0x39)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "CONVERT"), mrb_fixnum_value(0x79)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "KANA"), mrb_fixnum_value(0x70)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "RALT"), mrb_fixnum_value(0x38)); /* LALT + EXT */
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "MENU"), mrb_fixnum_value(0x5D)); /* X + EXT */
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "RCONTROL"), mrb_fixnum_value(0x1D)); /* LCONTROL + EXT */

          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "SCROLLROCK"), mrb_fixnum_value(0x46)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "HOME"), mrb_fixnum_value(0x47)); /* NUM7 + EXT */
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "UP"), mrb_fixnum_value(0x48)); /* NUM8 + EXT */
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "PAGEUP"), mrb_fixnum_value(0x49)); /* NUM9 + EXT */
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "LEFT"), mrb_fixnum_value(0x4B)); /* NUM4 + EXT */
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "RIGHT"), mrb_fixnum_value(0x4D)); /* NUM6 + EXT */
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "END"), mrb_fixnum_value(0x4F)); /* NUM1 + EXT */
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "DOWN"), mrb_fixnum_value(0x50)); /* NUM2 + EXT */
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "PAGEDOWN"), mrb_fixnum_value(0x51)); /* NUM3 + EXT */
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "INSERT"), mrb_fixnum_value(0x52)); /* NUM0 + EXT */
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "RWIN"), mrb_fixnum_value(0x5C)); /* X + EXT */

    
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "NUM7"), mrb_fixnum_value(0x47));
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "NUM8"), mrb_fixnum_value(0x48));
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "NUM9"), mrb_fixnum_value(0x49));

          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "NUMMINUS"), mrb_fixnum_value(0x4A)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "NUM4"), mrb_fixnum_value(0x4B));
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "NUM5"), mrb_fixnum_value(0x4C)); 
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "NUM6"), mrb_fixnum_value(0x4D));
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "NUMPLUS"), mrb_fixnum_value(0x4E));
    
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "NUM1"), mrb_fixnum_value(0x4F));
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "NUM2"), mrb_fixnum_value(0x50));
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "NUM3"), mrb_fixnum_value(0x51));

          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "NUM0"), mrb_fixnum_value(0x52));
   
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "NUMPERIOD"), mrb_fixnum_value(0x53));
          mrb_hash_set(mrb, scan, mrb_cstr_symbol(mrb, "NUMENTER"), mrb_fixnum_value(0x1C)); /* ENTER + EXT */
    }

    { 
      mrb_value virtual;
      virtual = mrb_hash_new(mrb);
      mrb_define_const(mrb, kbd, "Virtual", virtual);

      mrb_hash_set(mrb, virtual, mrb_nil_value(), mrb_fixnum_value(0x00));

      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "A"), mrb_fixnum_value('A'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "B"), mrb_fixnum_value('B'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "C"), mrb_fixnum_value('C'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "D"), mrb_fixnum_value('D'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "E"), mrb_fixnum_value('E'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "F"), mrb_fixnum_value('F'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "G"), mrb_fixnum_value('G'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "H"), mrb_fixnum_value('H'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "I"), mrb_fixnum_value('I'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "J"), mrb_fixnum_value('J'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "K"), mrb_fixnum_value('K'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "L"), mrb_fixnum_value('L'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "M"), mrb_fixnum_value('M'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "N"), mrb_fixnum_value('N'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "O"), mrb_fixnum_value('O'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "P"), mrb_fixnum_value('P'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "Q"), mrb_fixnum_value('Q'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "R"), mrb_fixnum_value('R'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "S"), mrb_fixnum_value('S'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "T"), mrb_fixnum_value('T'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "U"), mrb_fixnum_value('U'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "V"), mrb_fixnum_value('V'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "W"), mrb_fixnum_value('W'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "X"), mrb_fixnum_value('X'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "Y"), mrb_fixnum_value('Y'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "Z"), mrb_fixnum_value('Z'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "ZERO"), mrb_fixnum_value('0'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "ONE"), mrb_fixnum_value('1'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "TWO"), mrb_fixnum_value('2'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "THREE"), mrb_fixnum_value('3'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "FOUR"), mrb_fixnum_value('4'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "FIVE"), mrb_fixnum_value('5'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "SIX"), mrb_fixnum_value('6'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "SEVEN"), mrb_fixnum_value('7'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "EIGHT"), mrb_fixnum_value('8'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "NINE"), mrb_fixnum_value('9'));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "LBUTTON"), mrb_fixnum_value(VK_LBUTTON));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "RBUTTON"), mrb_fixnum_value(VK_RBUTTON));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "CANCEL"), mrb_fixnum_value(VK_CANCEL));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "MBUTTON"), mrb_fixnum_value(VK_MBUTTON));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "XBUTTON1"), mrb_fixnum_value(VK_XBUTTON1));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "XBUTTON2"), mrb_fixnum_value(VK_XBUTTON2));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "BACK"), mrb_fixnum_value(VK_BACK));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "TAB"), mrb_fixnum_value(VK_TAB));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "CLEAR"), mrb_fixnum_value(VK_CLEAR));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "RETURN"), mrb_fixnum_value(VK_RETURN));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "SHIFT"), mrb_fixnum_value(VK_SHIFT));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "CONTROL"), mrb_fixnum_value(VK_CONTROL));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "MENU"), mrb_fixnum_value(VK_MENU));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "PAUSE"), mrb_fixnum_value(VK_PAUSE));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "CAPITAL"), mrb_fixnum_value(VK_CAPITAL));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "KANA"), mrb_fixnum_value(VK_KANA));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "HANGEUL"), mrb_fixnum_value(VK_HANGEUL));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "HANGUL"), mrb_fixnum_value(VK_HANGUL));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "JUNJA"), mrb_fixnum_value(VK_JUNJA));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "FINAL"), mrb_fixnum_value(VK_FINAL));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "HANJA"), mrb_fixnum_value(VK_HANJA));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "KANJI"), mrb_fixnum_value(VK_KANJI));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "ESCAPE"), mrb_fixnum_value(VK_ESCAPE));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "CONVERT"), mrb_fixnum_value(VK_CONVERT));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "NONCONVERT"), mrb_fixnum_value(VK_NONCONVERT));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "ACCEPT"), mrb_fixnum_value(VK_ACCEPT));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "MODECHANGE"), mrb_fixnum_value(VK_MODECHANGE));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "SPACE"), mrb_fixnum_value(VK_SPACE));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "PRIOR"), mrb_fixnum_value(VK_PRIOR));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "NEXT"), mrb_fixnum_value(VK_NEXT));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "END"), mrb_fixnum_value(VK_END));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "HOME"), mrb_fixnum_value(VK_HOME));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "LEFT"), mrb_fixnum_value(VK_LEFT));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "UP"), mrb_fixnum_value(VK_UP));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "RIGHT"), mrb_fixnum_value(VK_RIGHT));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "DOWN"), mrb_fixnum_value(VK_DOWN));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "SELECT"), mrb_fixnum_value(VK_SELECT));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "PRINT"), mrb_fixnum_value(VK_PRINT));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "EXECUTE"), mrb_fixnum_value(VK_EXECUTE));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "SNAPSHOT"), mrb_fixnum_value(VK_SNAPSHOT));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "INSERT"), mrb_fixnum_value(VK_INSERT));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "DELETE"), mrb_fixnum_value(VK_DELETE));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "HELP"), mrb_fixnum_value(VK_HELP));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "LWIN"), mrb_fixnum_value(VK_LWIN));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "RWIN"), mrb_fixnum_value(VK_RWIN));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "APPS"), mrb_fixnum_value(VK_APPS));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "SLEEP"), mrb_fixnum_value(VK_SLEEP));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "NUMPAD0"), mrb_fixnum_value(VK_NUMPAD0));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "NUMPAD1"), mrb_fixnum_value(VK_NUMPAD1));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "NUMPAD2"), mrb_fixnum_value(VK_NUMPAD2));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "NUMPAD3"), mrb_fixnum_value(VK_NUMPAD3));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "NUMPAD4"), mrb_fixnum_value(VK_NUMPAD4));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "NUMPAD5"), mrb_fixnum_value(VK_NUMPAD5));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "NUMPAD6"), mrb_fixnum_value(VK_NUMPAD6));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "NUMPAD7"), mrb_fixnum_value(VK_NUMPAD7));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "NUMPAD8"), mrb_fixnum_value(VK_NUMPAD8));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "NUMPAD9"), mrb_fixnum_value(VK_NUMPAD9));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "MULTIPLY"), mrb_fixnum_value(VK_MULTIPLY));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "ADD"), mrb_fixnum_value(VK_ADD));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "SEPARATOR"), mrb_fixnum_value(VK_SEPARATOR));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "SUBTRACT"), mrb_fixnum_value(VK_SUBTRACT));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "DECIMAL"), mrb_fixnum_value(VK_DECIMAL));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "DIVIDE"), mrb_fixnum_value(VK_DIVIDE));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "F1"), mrb_fixnum_value(VK_F1));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "F2"), mrb_fixnum_value(VK_F2));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "F3"), mrb_fixnum_value(VK_F3));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "F4"), mrb_fixnum_value(VK_F4));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "F5"), mrb_fixnum_value(VK_F5));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "F6"), mrb_fixnum_value(VK_F6));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "F7"), mrb_fixnum_value(VK_F7));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "F8"), mrb_fixnum_value(VK_F8));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "F9"), mrb_fixnum_value(VK_F9));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "F10"), mrb_fixnum_value(VK_F10));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "F11"), mrb_fixnum_value(VK_F11));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "F12"), mrb_fixnum_value(VK_F12));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "F13"), mrb_fixnum_value(VK_F13));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "F14"), mrb_fixnum_value(VK_F14));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "F15"), mrb_fixnum_value(VK_F15));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "F16"), mrb_fixnum_value(VK_F16));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "F17"), mrb_fixnum_value(VK_F17));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "F18"), mrb_fixnum_value(VK_F18));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "F19"), mrb_fixnum_value(VK_F19));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "F20"), mrb_fixnum_value(VK_F20));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "F21"), mrb_fixnum_value(VK_F21));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "F22"), mrb_fixnum_value(VK_F22));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "F23"), mrb_fixnum_value(VK_F23));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "F24"), mrb_fixnum_value(VK_F24));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "NUMLOCK"), mrb_fixnum_value(VK_NUMLOCK));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "SCROLL"), mrb_fixnum_value(VK_SCROLL));
#ifdef NEC_VK
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_NEC_EQUAL"), mrb_fixnum_value(VK_OEM_NEC_EQUAL));
#endif
#ifdef FUJITSU_VK
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_FJ_JISHO"), mrb_fixnum_value(VK_OEM_FJ_JISHO));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_FJ_MASSHOU"), mrb_fixnum_value(VK_OEM_FJ_MASSHOU));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_FJ_TOUROKU"), mrb_fixnum_value(VK_OEM_FJ_TOUROKU));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_FJ_LOYA"), mrb_fixnum_value(VK_OEM_FJ_LOYA));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_FJ_ROYA"), mrb_fixnum_value(VK_OEM_FJ_ROYA));
#endif
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "LSHIFT"), mrb_fixnum_value(VK_LSHIFT));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "RSHIFT"), mrb_fixnum_value(VK_RSHIFT));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "LCONTROL"), mrb_fixnum_value(VK_LCONTROL));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "RCONTROL"), mrb_fixnum_value(VK_RCONTROL));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "LMENU"), mrb_fixnum_value(VK_LMENU));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "RMENU"), mrb_fixnum_value(VK_RMENU));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "BROWSER_BACK"), mrb_fixnum_value(VK_BROWSER_BACK));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "BROWSER_FORWARD"), mrb_fixnum_value(VK_BROWSER_FORWARD));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "BROWSER_REFRESH"), mrb_fixnum_value(VK_BROWSER_REFRESH));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "BROWSER_STOP"), mrb_fixnum_value(VK_BROWSER_STOP));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "BROWSER_SEARCH"), mrb_fixnum_value(VK_BROWSER_SEARCH));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "BROWSER_FAVORITES"), mrb_fixnum_value(VK_BROWSER_FAVORITES));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "BROWSER_HOME"), mrb_fixnum_value(VK_BROWSER_HOME));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "VOLUME_MUTE"), mrb_fixnum_value(VK_VOLUME_MUTE));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "VOLUME_DOWN"), mrb_fixnum_value(VK_VOLUME_DOWN));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "VOLUME_UP"), mrb_fixnum_value(VK_VOLUME_UP));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "MEDIA_NEXT_TRACK"), mrb_fixnum_value(VK_MEDIA_NEXT_TRACK));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "MEDIA_PREV_TRACK"), mrb_fixnum_value(VK_MEDIA_PREV_TRACK));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "MEDIA_STOP"), mrb_fixnum_value(VK_MEDIA_STOP));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "MEDIA_PLAY_PAUSE"), mrb_fixnum_value(VK_MEDIA_PLAY_PAUSE));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "LAUNCH_MAIL"), mrb_fixnum_value(VK_LAUNCH_MAIL));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "LAUNCH_MEDIA_SELECT"), mrb_fixnum_value(VK_LAUNCH_MEDIA_SELECT));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "LAUNCH_APP1"), mrb_fixnum_value(VK_LAUNCH_APP1));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "LAUNCH_APP2"), mrb_fixnum_value(VK_LAUNCH_APP2));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_1"), mrb_fixnum_value(VK_OEM_1));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_PLUS"), mrb_fixnum_value(VK_OEM_PLUS));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_COMMA"), mrb_fixnum_value(VK_OEM_COMMA));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_MINUS"), mrb_fixnum_value(VK_OEM_MINUS));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_PERIOD"), mrb_fixnum_value(VK_OEM_PERIOD));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_2"), mrb_fixnum_value(VK_OEM_2));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_3"), mrb_fixnum_value(VK_OEM_3));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_4"), mrb_fixnum_value(VK_OEM_4));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_5"), mrb_fixnum_value(VK_OEM_5));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_6"), mrb_fixnum_value(VK_OEM_6));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_7"), mrb_fixnum_value(VK_OEM_7));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_8"), mrb_fixnum_value(VK_OEM_8));
      /*     mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_AX"), mrb_fixnum_value(VK_OEM_AX)); */
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_102"), mrb_fixnum_value(VK_OEM_102));
      /*     mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "ICO_HELP"), mrb_fixnum_value(VK_ICO_HELP)); */
      /*     mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "ICO_00"), mrb_fixnum_value(VK_ICO_00)); */
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "PROCESSKEY"), mrb_fixnum_value(VK_PROCESSKEY));
      /*     mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "ICO_CLEAR"), mrb_fixnum_value(VK_ICO_CLEAR)); */
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "PACKET"), mrb_fixnum_value(VK_PACKET));
      /*     mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_RESET"), mrb_fixnum_value(VK_OEM_RESET)); */
      /*     mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_JUMP"), mrb_fixnum_value(VK_OEM_JUMP)); */
      /*     mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_PA1"), mrb_fixnum_value(VK_OEM_PA1)); */
      /*     mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_PA2"), mrb_fixnum_value(VK_OEM_PA2)); */
      /*     mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_PA3"), mrb_fixnum_value(VK_OEM_PA3)); */
      /*     mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_WSCTRL"), mrb_fixnum_value(VK_OEM_WSCTRL)); */
      /*     mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_CUSEL"), mrb_fixnum_value(VK_OEM_CUSEL)); */
      /*     mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_ATTN"), mrb_fixnum_value(VK_OEM_ATTN)); */
      /*     mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_FINISH"), mrb_fixnum_value(VK_OEM_FINISH)); */
      /*     mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_COPY"), mrb_fixnum_value(VK_OEM_COPY)); */
      /*     mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_AUTO"), mrb_fixnum_value(VK_OEM_AUTO)); */
      /*     mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_ENLW"), mrb_fixnum_value(VK_OEM_ENLW)); */
      /*     mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_BACKTAB"), mrb_fixnum_value(VK_OEM_BACKTAB)); */
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "ATTN"), mrb_fixnum_value(VK_ATTN));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "CRSEL"), mrb_fixnum_value(VK_CRSEL));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "EXSEL"), mrb_fixnum_value(VK_EXSEL));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "EREOF"), mrb_fixnum_value(VK_EREOF));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "PLAY"), mrb_fixnum_value(VK_PLAY));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "ZOOM"), mrb_fixnum_value(VK_ZOOM));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "NONAME"), mrb_fixnum_value(VK_NONAME));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "PA1"), mrb_fixnum_value(VK_PA1));
      mrb_hash_set(mrb, virtual, mrb_cstr_symbol(mrb, "OEM_CLEAR"), mrb_fixnum_value(VK_OEM_CLEAR));
    }    
}

void
mrb_mruby_winapi_keyboard_gem_final(mrb_state* mrb)
{
}
