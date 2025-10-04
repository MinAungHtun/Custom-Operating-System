#ifndef __DRIVERS__KEYBOARD__KEYBOARDINTERPRETER_H
    #define __DRIVERS__KEYBOARD__KEYBOARDINTERPRETER_H

    #include<common/types.h>
    #include<common/vector.h>
    #include<common/inputstream.h>
    #include<drivers/keyboard/keyboard.h>

    namespace MYYOS
    {
        namespace drivers
        {
            namespace keyboard
            {

                enum KeyCode : uint16_t
                {
                    A = 'A', B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
                    a = 'a', b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z,
                    K0 = '0', K1, K2, K3, K4, K5, K6, K7, K8, K9,

                    Comma           = ',',
                    Period          = '.',
                    Exclamationmark = '!',
                    Questionmark    = '?',
                    Quotationmark   = '\"',
                    Semicolon       = ';',
                    Colon           = ':',
                    Apostrophe      = '\'',

                    Circumflex      = '^',
                    Dollarsign      = '$',
                    Percentsign     = '%',
                    Ampersand       = '&',
                    Underscore      = '_',
                    Pipe            = '|',
                    Numbersign      = '#',
                    Backslash       = '\\',
                    At              = '@',
                    Tilde           = '~',

                    Plus            = '+',
                    Minus           = '-',
                    Multiplication  = '*',
                    Slash           = '/',
                    Equalsign       = '=',
                    LessThan        = '<', GreaterThan      = '>',
                    OpenParenthesis = '(', CloseParenthesis = ')',
                    OpenBracket     = '[', CloseBracket     = ']',
                    OpenBrace       = '{', CloseBrace       = '}',

                    Tabulator       = '\t',
                    Space           = ' ',
                    Return          = '\n',
                    Backspace       = '\b',

                    Escape          = 0x1b,
                    Delete          = 0x7f,
                    ScrollDown      = 0x0c,

                    F1 = 0xF0, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
                    Home, End, ScrollUp, Insert,
                    CapitalsLock, LeftShift, RightShift,
                    LeftControl, LeftMeta, Alternate, AlternateGroup, RightMeta, Context, RightControl,
                    ArrowUp, ArrowDown, ArrowLeft, ArrowRight,

                    NumberLock, ScrollLock, Pause, Print,
                    KeypadDivision, KeypadMultiplication, KeypadMinus, KeypadPlus, KeypadComma, KeypadReturn,
                    Keypad0, Keypad1, Keypad2, Keypad3, Keypad4, Keypad5, Keypad6, Keypad7, Keypad8, Keypad9,
                    KeypadInsert, KeypadDelete, KeypadEnd, KeypadArrowDown, KeypadScrollDown, KeypadArrowLeft,
                    KeypadArrowRight, KeypadHome, KeypadArrowUp, KeypadScrollUp,

                    Degree, Paragraph, Power2, Power3,
                    _uuml, _auml, _ouml, _Uuml, _Auml, _Ouml, _szlig,
                    _acute, _grave, _eurosign, _mu
                };


                class KeyboardState
                {
                    public:
                        KeyboardState();
                        ~KeyboardState();

                        bool LeftShift;
                        bool RightShift;
                        bool CapitalsLock;
                        bool LeftMeta;
                        bool RightMeta;
                        bool LeftControl;
                        bool RightControl;
                        bool Alternate;
                        bool AlternateGroup;
                        bool NumberLock;
                        bool ScrollLock;
                };

                class KeyboardEventHandler
                {
                    public:
                        KeyboardEventHandler();
                        ~KeyboardEventHandler();
                        virtual void OnKeyboardKeyPressed(KeyCode keyCode, KeyboardState state);
                        virtual void OnKeyboardKeyReleased(KeyCode keyCode, KeyboardState state);
                };

                class KeyboardInterpreter : public common::InputStreamEventHandler<uint8_t>
                {
                    protected:
                        bool e0Code;
                        uint8_t e1Code;
                        uint16_t e1CodeBuffer;

                        KeyboardState state;
                        common::Vector<KeyboardEventHandler*> handlers;

                    public:
                        KeyboardInterpreter();
                        ~KeyboardInterpreter();
                        void ConnectEventHandler(KeyboardEventHandler* handler);
                        void OnRead(bool released, KeyboardState state, KeyCode keyCode);
                };

                class KeyboardInterpreterDE : public KeyboardInterpreter
                {
                    public:

                        enum KeyCodeDE
                        {
                            Escape = 1, F1 = 59, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11 = 87, F12, ScrollLock = 70,
                            Circumflex = 41, K1 = 2, K2, K3, K4, K5, K6, K7, K8, K9, K0, _szlig, _acute, Backspace,
                            Tabulator = 15, Q, W, E, R, T, Z, U, I, O, P, _uuml, Plus, Return,
                            CapitalsLock = 58, A = 30, S, D, F, G, H, J, K, L, _ouml, _auml, Numbersign = 43,
                            LeftShift = 42, LessThan = 86, Y = 44, X, C, V, B, N, M, Comma, Period, Minus, RightShift,
                            LeftControl = 29, Alternate = 56, Space,

                            NumberLock = 69, KeypadMultiplication = 55, KeypadMinus = 74,
                            Keypad7 = 71, Keypad8, Keypad9, KeypadPlus = 78,
                            Keypad4 = 75, Keypad5, Keypad6,
                            Keypad1 = 79, Keypad2, Keypad3,
                            Keypad0 = 82, KeypadComma = 83,
                        };

                        enum KeyCodeDEExtended1
                        {
                            //PrintScreen    = 0xb7,
                            KeypadDivision = 0x35,
                            KeypadReturn   = 0x1c,

                            LeftMeta = 0x5b, RightMeta, Context, AlternateGroup = 0x38, RightControl = 0x1d,

                            ArrowUp = 0x48, ArrowLeft = 0x4b, ArrowDown = 0x50, ArrowRight = 0x4d,

                            Insert = 0x52, Home = 0x47, ScrollUp = 0x49,
                            Delete = 0x53, End = 0x4f, ScrollDown = 0x51,
                        };

                        /*
                        enum KeyCodeDEExtended2 : uint16_t
                        {
                            Pause          = 0x451d
                        };
                        */

                        KeyboardInterpreterDE();
                        ~KeyboardInterpreterDE();
                        void OnRead(uint8_t scancode);
                };

            }
        }
    }

#endif