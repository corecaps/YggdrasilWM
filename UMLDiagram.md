@startuml
class Logger{
- logStream_ : std::ofstream
- logLevel_ : LogLevel
{method} + Logger(log_file: std::string, log_level: int) : Logger
{method} + ~Logger() : void
{method} + log(level: int, message: std::string) : void
{method} - GetLogLevel() : std::string
{method} - GetTime() : std::string
}
class WindowManager {
    - Logger : Logger
    - wm_detected_ : bool
    - display_ : Display*
    - root_ : Window
    - clients_ : std::vector<Client>
    - WM_PROTOCOLS_ : Atom
    - WM_DELETE_WINDOW_ : Atom
{method} + Run() : void
{method} + Create(Logger: Logger, display: std::string) : std::unique_ptr<window_manager>
{method} - WindowManager(display: std::string, Logger : Logger) : std::unique_ptr<window_manager>
{method} - OnXError(error: XErrorEvent*) : void
{method} - OnXMapping(mapping: XMappingEvent*) : void
}
class Client {
    - window_ : Window
    - x_ : int
    - y_ : int
    - width_ : int
    - height_ : int
    - border_width_ : int
    - depth_ : int
    - mapped_ : bool
    - override_redirect_ : bool
    - wm_state_ : Atom
    - wm_state_fullscreen_ : Atom
    - wm_state_maximized_horz_ : Atom
    - wm_state_maximized_vert_ : Atom
    - wm_state_hidden_ : Atom
    - wm_state_modal_ : Atom
    - wm_state_above_ : Atom
    - wm_state_below_ : Atom
    - wm_state_sticky_ : Atom
    - wm_state_shaded_ : Atom
    - wm_state_skip_taskbar_ : Atom
    - wm_state_skip_pager_ : Atom
    - wm_state_fullscreen_ : Atom
    - wm_state_maximized_horz_ : Atom
    - wm_state_maximized_vert_ : Atom
    - wm_state_hidden_ : Atom
    - wm_state_modal_ : Atom
    - wm_state_above_ : Atom
    - wm_state_below_ : Atom
    - wm_state_sticky_ : Atom
    - wm_state_shaded_ : Atom
    - wm_state_skip_taskbar_ : Atom
    - wm_state_skip_pager_ : Atom
    - wm_state_fullscreen_ : Atom
    - wm_state_maximized_horz_ : Atom
    - wm_state_maximized_vert_ : Atom
    - wm_state_hidden_ : Atom
    - wm_state_modal_ : Atom
    - wm_state_above_ : Atom
    - wm_state_below_ : Atom
    - wm_state_sticky_ : Atom
    - wm_state_shaded_ : Atom
    - wm_state_skip_taskbar_ : Atom
    - wm_state_skip_pager_ : Atom
    - wm_state_fullscreen_ : Atom
    - wm_state_maximized_horz_ : Atom
    - wm_state_maximized_vert_ : Atom
    - wm_state_hidden_ : Atom
    - wm_state_modal_ : Atom
    - wm_state_above_ : Atom
    - wm_state_below_ : Atom
    - wm_state_sticky_ : Atom
    - wm_state_shaded_ : Atom
}
Logger *-- WindowManager
WindowManager *--  Client
@enduml
