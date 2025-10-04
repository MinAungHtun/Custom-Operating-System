
#include <drivers/keyboard/keyboardinterpreter.h>

using namespace MYYOS::common;

namespace MYYOS
{
    namespace drivers
    {
        namespace keyboard
        {


            KeyboardState::KeyboardState()
            {
                LeftShift      = false;
                RightShift     = false;
                CapitalsLock   = false;
                LeftMeta       = false;
                RightMeta      = false;
                LeftControl    = false;
                RightControl   = false;
                Alternate      = false;
                AlternateGroup = false;
                NumberLock     = false;
                ScrollLock     = false;
            }

            KeyboardState::~KeyboardState()
            {
            }





            KeyboardEventHandler::KeyboardEventHandler()
            {
            }

            KeyboardEventHandler::~KeyboardEventHandler()
            {
            }

            void KeyboardEventHandler::OnKeyboardKeyPressed(KeyCode, KeyboardState)
            {
            }

            void KeyboardEventHandler::OnKeyboardKeyReleased(KeyCode, KeyboardState)
            {
            }





            KeyboardInterpreter::KeyboardInterpreter()
                : InputStreamEventHandler<uint8_t>::InputStreamEventHandler()
            {
                e0Code = false;
                e1Code = 0;
                e1CodeBuffer = 0;
            }

            KeyboardInterpreter::~KeyboardInterpreter()
            {
            }

            void KeyboardInterpreter::ConnectEventHandler(KeyboardEventHandler* handler)
            {
                handlers.push_back(handler);
            }

            void KeyboardInterpreter::OnRead(bool released, KeyboardState state, KeyCode keyCode)
            {
                if(!released)
                    for(Vector<KeyboardEventHandler*>::iterator handler = handlers.begin(); handler != handlers.end(); handler++)
                        (*handler)->OnKeyboardKeyReleased(keyCode, state);
                else
                    for(Vector<KeyboardEventHandler*>::iterator handler = handlers.begin(); handler != handlers.end(); handler++)
                        (*handler)->OnKeyboardKeyPressed(keyCode, state);
            }




            KeyboardInterpreterDE::KeyboardInterpreterDE()
                : KeyboardInterpreter()
            {
            }

            KeyboardInterpreterDE::~KeyboardInterpreterDE()
            {
            }

            void KeyboardInterpreterDE::OnRead(uint8_t scancode)
            {
                int keyType = 0;
                bool released = (scancode & 0x80)
                              && (e1Code || (scancode != 0xe1))
                              && (e0Code || (scancode != 0xe0));
                if(released)
                    scancode &= ~0x80;


                if(scancode == 0xe0)
                {
                    e0Code = true;
                    return;
                }
                if(e0Code)
                {
                    keyType = 1;
                    e0Code = false;

                    // "fake shift"
                    if((KeyboardInterpreterDE::KeyCodeDE)scancode == KeyboardInterpreterDE::LeftShift
                    || (KeyboardInterpreterDE::KeyCodeDE)scancode == KeyboardInterpreterDE::RightShift)
                        return;
                }

                if(scancode == 0xe1)
                {
                    e1Code = 1;
                    return;
                }
                if(e1Code == 1)
                {
                    e1Code = 2;
                    e1CodeBuffer = scancode;
                    return;
                }
                if(e1Code == 2)
                {
                    keyType = 2;
                    e1Code = 0;
                    e1CodeBuffer |= (((uint16_t)scancode) << 8);
                }


                bool shift = state.LeftShift || state.RightShift;
                bool uppercase = shift != state.CapitalsLock;

                if(keyType == 0)
                {
                    switch((KeyboardInterpreterDE::KeyCodeDE)scancode)
                    {
                        case KeyCodeDE::Q :
                            if(state.AlternateGroup)          KeyboardInterpreter::OnRead(released, state, keyboard::At);
                            else                              KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::Q : keyboard::q);                             break;
                        case KeyCodeDE::W :                   KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::W : keyboard::w);                             break;
                        case KeyCodeDE::E :
                            if(state.AlternateGroup)          KeyboardInterpreter::OnRead(released, state, keyboard::_eurosign);
                            else                              KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::E : keyboard::e);                             break;
                        case KeyCodeDE::R :                   KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::R : keyboard::r);                             break;
                        case KeyCodeDE::T :                   KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::T : keyboard::t);                             break;
                        case KeyCodeDE::Z :                   KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::Z : keyboard::z);                             break;
                        case KeyCodeDE::U :                   KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::U : keyboard::u);                             break;
                        case KeyCodeDE::I :                   KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::I : keyboard::i);                             break;
                        case KeyCodeDE::O :                   KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::O : keyboard::o);                             break;
                        case KeyCodeDE::P :                   KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::P : keyboard::p);                             break;
                        case KeyCodeDE::A :                   KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::A : keyboard::a);                             break;
                        case KeyCodeDE::S :                   KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::S : keyboard::s);                             break;
                        case KeyCodeDE::D :                   KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::D : keyboard::d);                             break;
                        case KeyCodeDE::F :                   KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::F : keyboard::f);                             break;
                        case KeyCodeDE::G :                   KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::G : keyboard::g);                             break;
                        case KeyCodeDE::H :                   KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::H : keyboard::h);                             break;
                        case KeyCodeDE::J :                   KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::J : keyboard::j);                             break;
                        case KeyCodeDE::K :                   KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::K : keyboard::k);                             break;
                        case KeyCodeDE::L :                   KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::L : keyboard::l);                             break;
                        case KeyCodeDE::Y :                   KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::Y : keyboard::y);                             break;
                        case KeyCodeDE::X :                   KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::X : keyboard::x);                             break;
                        case KeyCodeDE::C :                   KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::C : keyboard::c);                             break;
                        case KeyCodeDE::V :                   KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::V : keyboard::v);                             break;
                        case KeyCodeDE::B :                   KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::B : keyboard::b);                             break;
                        case KeyCodeDE::N :                   KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::N : keyboard::n);                             break;
                        case KeyCodeDE::M :
                            if(state.AlternateGroup)          KeyboardInterpreter::OnRead(released, state, keyboard::_mu);
                            else                              KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::M : keyboard::m);                             break;
                        case KeyCodeDE::_auml:                KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::_Auml : keyboard::_auml);                     break;
                        case KeyCodeDE::_ouml:                KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::_Ouml : keyboard::_ouml);                     break;
                        case KeyCodeDE::_uuml:                KeyboardInterpreter::OnRead(released, state, uppercase ? keyboard::_Uuml : keyboard::_uuml);                     break;

                        case KeyCodeDE::Escape:               KeyboardInterpreter::OnRead(released, state, keyboard::Escape);                                                  break;
                        case KeyCodeDE::F1:                   KeyboardInterpreter::OnRead(released, state, keyboard::F1);                                                      break;
                        case KeyCodeDE::F2:                   KeyboardInterpreter::OnRead(released, state, keyboard::F2);                                                      break;
                        case KeyCodeDE::F3:                   KeyboardInterpreter::OnRead(released, state, keyboard::F3);                                                      break;
                        case KeyCodeDE::F4:                   KeyboardInterpreter::OnRead(released, state, keyboard::F4);                                                      break;
                        case KeyCodeDE::F5:                   KeyboardInterpreter::OnRead(released, state, keyboard::F5);                                                      break;
                        case KeyCodeDE::F6:                   KeyboardInterpreter::OnRead(released, state, keyboard::F6);                                                      break;
                        case KeyCodeDE::F7:                   KeyboardInterpreter::OnRead(released, state, keyboard::F7);                                                      break;
                        case KeyCodeDE::F8:                   KeyboardInterpreter::OnRead(released, state, keyboard::F8);                                                      break;
                        case KeyCodeDE::F9:                   KeyboardInterpreter::OnRead(released, state, keyboard::F9);                                                      break;
                        case KeyCodeDE::F10:                  KeyboardInterpreter::OnRead(released, state, keyboard::F10);                                                     break;
                        case KeyCodeDE::F11:                  KeyboardInterpreter::OnRead(released, state, keyboard::F11);                                                     break;
                        case KeyCodeDE::F12:                  KeyboardInterpreter::OnRead(released, state, keyboard::F12);                                                     break;
                        case KeyCodeDE::K1:                   KeyboardInterpreter::OnRead(released, state, !shift ? keyboard::K1 : keyboard::Exclamationmark);                 break;
                        case KeyCodeDE::K2:
                            if(state.AlternateGroup)          KeyboardInterpreter::OnRead(released, state, keyboard::Power2);
                            else                              KeyboardInterpreter::OnRead(released, state, !shift ? keyboard::K2 : keyboard::Quotationmark);                   break;
                        case KeyCodeDE::K3:
                            if(state.AlternateGroup)          KeyboardInterpreter::OnRead(released, state, keyboard::Power3);
                            else                              KeyboardInterpreter::OnRead(released, state, !shift ? keyboard::K3 : keyboard::Paragraph);                       break;
                        case KeyCodeDE::K4:                   KeyboardInterpreter::OnRead(released, state, !shift ? keyboard::K4 : keyboard::Dollarsign);                      break;
                        case KeyCodeDE::K5:                   KeyboardInterpreter::OnRead(released, state, !shift ? keyboard::K5 : keyboard::Percentsign);                     break;
                        case KeyCodeDE::K6:                   KeyboardInterpreter::OnRead(released, state, !shift ? keyboard::K6 : keyboard::Ampersand);                       break;
                        case KeyCodeDE::K7:
                            if(state.AlternateGroup)          KeyboardInterpreter::OnRead(released, state, keyboard::OpenBrace);
                            else                              KeyboardInterpreter::OnRead(released, state, !shift ? keyboard::K7 : keyboard::Slash);                           break;
                        case KeyCodeDE::K8:
                            if(state.AlternateGroup)          KeyboardInterpreter::OnRead(released, state, keyboard::OpenBracket);
                            else                              KeyboardInterpreter::OnRead(released, state, !shift ? keyboard::K8 : keyboard::OpenParenthesis);                 break;
                        case KeyCodeDE::K9:
                            if(state.AlternateGroup)          KeyboardInterpreter::OnRead(released, state, keyboard::CloseBracket);
                            else                              KeyboardInterpreter::OnRead(released, state, !shift ? keyboard::K9 : keyboard::CloseParenthesis);                break;
                        case KeyCodeDE::K0:
                            if(state.AlternateGroup)          KeyboardInterpreter::OnRead(released, state, keyboard::CloseBrace);
                            else                              KeyboardInterpreter::OnRead(released, state, !shift ? keyboard::K0 : keyboard::Equalsign);                       break;
                        case KeyCodeDE::_szlig:
                            if(state.AlternateGroup)          KeyboardInterpreter::OnRead(released, state, keyboard::Backslash);
                            else                              KeyboardInterpreter::OnRead(released, state, !shift ? keyboard::_szlig : keyboard::Questionmark);                break;
                        case KeyCodeDE::Plus:
                            if(state.AlternateGroup)          KeyboardInterpreter::OnRead(released, state, keyboard::Tilde);
                            else                              KeyboardInterpreter::OnRead(released, state, !shift ? keyboard::Plus : keyboard::Multiplication);                break;
                        case KeyCodeDE::LessThan:
                            if(state.AlternateGroup)          KeyboardInterpreter::OnRead(released, state, keyboard::Pipe);
                            else                              KeyboardInterpreter::OnRead(released, state, !shift ? keyboard::LessThan : keyboard::GreaterThan);               break;
                        case KeyCodeDE::Numbersign:           KeyboardInterpreter::OnRead(released, state, !shift ? keyboard::Apostrophe : keyboard::Numbersign);              break;
                        case KeyCodeDE::Comma:                KeyboardInterpreter::OnRead(released, state, !shift ? keyboard::Comma  : keyboard::Semicolon);                   break;
                        case KeyCodeDE::Period:               KeyboardInterpreter::OnRead(released, state, !shift ? keyboard::Period : keyboard::Colon);                       break;
                        case KeyCodeDE::Minus:                KeyboardInterpreter::OnRead(released, state, !shift ? keyboard::Minus  : keyboard::Underscore);                  break;
                        case KeyCodeDE::Backspace:            KeyboardInterpreter::OnRead(released, state, keyboard::Backspace);                                               break;
                        case KeyCodeDE::Space:                KeyboardInterpreter::OnRead(released, state, keyboard::Space);                                                   break;
                        case KeyCodeDE::Return:               KeyboardInterpreter::OnRead(released, state, keyboard::Return);                                                  break;
                        case KeyCodeDE::Tabulator:            KeyboardInterpreter::OnRead(released, state, keyboard::Tabulator);                                               break;
                        // lock keys
                        case KeyCodeDE::CapitalsLock:         if(!released) state.CapitalsLock = !state.CapitalsLock;
                                                              KeyboardInterpreter::OnRead(released, state, keyboard::CapitalsLock);                                            break;
                        case KeyCodeDE::NumberLock:           if(!released) state.NumberLock   = !state.NumberLock;
                                                              KeyboardInterpreter::OnRead(released, state, keyboard::NumberLock);                                              break;
                        case KeyCodeDE::ScrollLock:           if(!released) state.ScrollLock   = !state.ScrollLock;
                                                              this->KeyboardInterpreter::OnRead(released, state, keyboard::ScrollLock);                                        break;
                        // dead keys
                        case KeyCodeDE::LeftShift:            state.LeftShift      = !released;
                                                              KeyboardInterpreter::OnRead(released, state, keyboard::LeftShift);                                               break;
                        case KeyCodeDE::RightShift:           state.RightShift     = !released;
                                                              KeyboardInterpreter::OnRead(released, state, keyboard::RightShift);                                              break;
                        case KeyCodeDE::LeftControl:          state.LeftControl    = !released;
                                                              KeyboardInterpreter::OnRead(released, state, keyboard::LeftControl);                                             break;
                        case KeyCodeDE::Alternate:            state.Alternate      = !released;
                                                              KeyboardInterpreter::OnRead(released, state, keyboard::Alternate);                                               break;
                        case KeyCodeDE::Circumflex:           KeyboardInterpreter::OnRead(released, state, !shift ? keyboard::Circumflex : keyboard::Degree);                  break;
                        case KeyCodeDE::_acute:               KeyboardInterpreter::OnRead(released, state, !shift ? keyboard::_acute  : keyboard::_grave);                     break;

                        case KeyCodeDE::Keypad0:              KeyboardInterpreter::OnRead(released, state, state.NumberLock ? keyboard::Keypad0 : keyboard::KeypadInsert);     break;
                        case KeyCodeDE::Keypad1:              KeyboardInterpreter::OnRead(released, state, state.NumberLock ? keyboard::Keypad1 : keyboard::KeypadEnd);        break;
                        case KeyCodeDE::Keypad2:              KeyboardInterpreter::OnRead(released, state, state.NumberLock ? keyboard::Keypad2 : keyboard::KeypadArrowDown);  break;
                        case KeyCodeDE::Keypad3:              KeyboardInterpreter::OnRead(released, state, state.NumberLock ? keyboard::Keypad3 : keyboard::KeypadScrollDown); break;
                        case KeyCodeDE::Keypad4:              KeyboardInterpreter::OnRead(released, state, state.NumberLock ? keyboard::Keypad4 : keyboard::KeypadArrowLeft);  break;
                        case KeyCodeDE::Keypad5:              KeyboardInterpreter::OnRead(released, state,                    keyboard::Keypad5);                              break;
                        case KeyCodeDE::Keypad6:              KeyboardInterpreter::OnRead(released, state, state.NumberLock ? keyboard::Keypad6 : keyboard::KeypadArrowRight); break;
                        case KeyCodeDE::Keypad7:              KeyboardInterpreter::OnRead(released, state, state.NumberLock ? keyboard::Keypad7 : keyboard::KeypadHome);       break;
                        case KeyCodeDE::Keypad8:              KeyboardInterpreter::OnRead(released, state, state.NumberLock ? keyboard::Keypad8 : keyboard::KeypadArrowUp);    break;
                        case KeyCodeDE::Keypad9:              KeyboardInterpreter::OnRead(released, state, state.NumberLock ? keyboard::Keypad9 : keyboard::KeypadScrollUp);   break;
                        case KeyCodeDE::KeypadMultiplication: KeyboardInterpreter::OnRead(released, state,                    keyboard::KeypadMultiplication);                 break;
                        case KeyCodeDE::KeypadPlus:           KeyboardInterpreter::OnRead(released, state,                    keyboard::KeypadPlus);                           break;
                        case KeyCodeDE::KeypadMinus:          KeyboardInterpreter::OnRead(released, state,                    keyboard::KeypadMinus);                          break;
                        case KeyCodeDE::KeypadComma:          KeyboardInterpreter::OnRead(released, state, state.NumberLock ? keyboard::KeypadComma : keyboard::KeypadDelete); break;
                        //default: break;
                    }
                }
                else if(keyType == 1)
                {
                    switch((KeyCodeDEExtended1)scancode)
                    {
                        //case KeyCodeDEExtended1::PrintScreen:
                        case KeyCodeDEExtended1::KeypadReturn:                                       KeyboardInterpreter::OnRead(released, state, keyboard::KeypadReturn);     break;
                        case KeyCodeDEExtended1::RightControl:    state.RightControl   = !released;  KeyboardInterpreter::OnRead(released, state, keyboard::RightControl);     break;
                        case KeyCodeDEExtended1::KeypadDivision:                                     KeyboardInterpreter::OnRead(released, state, keyboard::KeypadDivision);   break;
                        case KeyCodeDEExtended1::AlternateGroup:  state.AlternateGroup = !released;  KeyboardInterpreter::OnRead(released, state, keyboard::AlternateGroup);   break;
                        case KeyCodeDEExtended1::Home:                                               KeyboardInterpreter::OnRead(released, state, keyboard::Home);             break;
                        case KeyCodeDEExtended1::ArrowUp:                                            KeyboardInterpreter::OnRead(released, state, keyboard::ArrowUp);          break;
                        case KeyCodeDEExtended1::ScrollUp:                                           KeyboardInterpreter::OnRead(released, state, keyboard::ScrollUp);         break;
                        case KeyCodeDEExtended1::ArrowLeft:                                          KeyboardInterpreter::OnRead(released, state, keyboard::ArrowLeft);        break;
                        case KeyCodeDEExtended1::ArrowRight:                                         KeyboardInterpreter::OnRead(released, state, keyboard::ArrowRight);       break;
                        case KeyCodeDEExtended1::End:                                                KeyboardInterpreter::OnRead(released, state, keyboard::End);              break;
                        case KeyCodeDEExtended1::ArrowDown:                                          KeyboardInterpreter::OnRead(released, state, keyboard::ArrowDown);        break;
                        case KeyCodeDEExtended1::ScrollDown:                                         KeyboardInterpreter::OnRead(released, state, keyboard::ScrollDown);       break;
                        case KeyCodeDEExtended1::Insert:                                             KeyboardInterpreter::OnRead(released, state, keyboard::Insert);           break;
                        case KeyCodeDEExtended1::Delete:                                             KeyboardInterpreter::OnRead(released, state, keyboard::Delete);           break;
                        case KeyCodeDEExtended1::LeftMeta:        state.LeftMeta       = !released;  KeyboardInterpreter::OnRead(released, state, keyboard::LeftMeta);         break;
                        case KeyCodeDEExtended1::RightMeta:       state.RightMeta      = !released;  KeyboardInterpreter::OnRead(released, state, keyboard::RightMeta);        break;
                        case KeyCodeDEExtended1::Context:                                            KeyboardInterpreter::OnRead(released, state, keyboard::Context);          break;
                    }

                }
                else
                {
                    // to be continued...
                }
            }
        }
    }
}