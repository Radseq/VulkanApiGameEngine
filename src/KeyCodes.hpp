#ifndef KEY_CODES_HPP
#define KEY_CODES_HPP

#define KEY_ESCAPE GLFW_KEY_ESCAPE
#define KEY_F1     GLFW_KEY_F1
#define KEY_F2     GLFW_KEY_F2
#define KEY_F3     GLFW_KEY_F3
#define KEY_F4     GLFW_KEY_F4
#define KEY_F5     GLFW_KEY_F5
#define KEY_A      GLFW_KEY_A
#define KEY_B      GLFW_KEY_B
#define KEY_D      GLFW_KEY_D
#define KEY_F      GLFW_KEY_F
#define KEY_G      GLFW_KEY_G
#define KEY_L      GLFW_KEY_L
#define KEY_N      GLFW_KEY_N
#define KEY_O      GLFW_KEY_O
#define KEY_P      GLFW_KEY_P
#define KEY_S      GLFW_KEY_S
#define KEY_T      GLFW_KEY_T
#define KEY_W      GLFW_KEY_W
#define KEY_SPACE  GLFW_KEY_SPACE
#define KEY_KPADD  GLFW_KEY_KP_ADD
#define KEY_KPSUB  GLFW_KEY_KP_SUBTRACT

#define GAMEPAD_BUTTON_A     0x1000
#define GAMEPAD_BUTTON_B     0x1001
#define GAMEPAD_BUTTON_X     0x1002
#define GAMEPAD_BUTTON_Y     0x1003
#define GAMEPAD_BUTTON_L1    0x1004
#define GAMEPAD_BUTTON_R1    0x1005
#define GAMEPAD_BUTTON_START 0x1006

class KeyCodes
{
    struct KeyBoard
    {
        bool esc   = false;
        bool f1    = false;
        bool f2    = false;
        bool f3    = false;
        bool f4    = false;
        bool f5    = false;
        bool a     = false;
        bool b     = false;
        bool d     = false;
        bool f     = false;
        bool g     = false;
        bool l     = false;
        bool n     = false;
        bool o     = false;
        bool p     = false;
        bool s     = false;
        bool t     = false;
        bool w     = false;
        bool space = false;
        // bool KEY_KPADD = false;
        // bool KEY_KPSUB = false;
    };

    struct Mouse
    {
        bool      left   = false;
        bool      right  = false;
        bool      middle = false;
        float     scrollDeta {0};
        bool      isMouseScrooling {false};
        glm::vec2 deltaPos {0, 0};
    };

   public:
    static KeyCodes& Instance( )
    {
        // Since it's a static variable, if the class has already been created,
        // it won't be created again.
        // And it **is** thread-safe in C++11.
        static KeyCodes myInstance;

        // Return a reference to our instance.
        return myInstance;
    }

    // delete copy and move constructors and assign operators
    KeyCodes (KeyCodes const&) = delete;             // Copy construct
    KeyCodes (KeyCodes&&)      = delete;             // Move construct
    KeyCodes& operator= (KeyCodes const&) = delete;  // Copy assign
    KeyCodes& operator= (KeyCodes&&) = delete;       // Move assign

    // Any other public methods.

    KeyBoard keyBoard { };
    Mouse    mouse { };

   protected:
    KeyCodes( )
    {
        // Constructor code goes here.
    }

    ~KeyCodes( )
    {
        // Destructor code goes here.
    }

    // And any other protected methods.
};

#endif  // KEY_CODES_HPP
