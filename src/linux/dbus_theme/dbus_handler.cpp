#include "dbus_handler.hpp"


#include <system_theme_pp/system_theme.hpp>

namespace system_theme_pp {
    DBusMessage* DebusHandler::getAppearance(const char* key) {
        DBusError err;
        dbus_error_init(&err);

        if(!signalConn) {
            signalConn = dbus_bus_get(DBUS_BUS_SESSION, &err);
        }
        if(dbus_error_is_set(&err) || !signalConn) {
            dbus_error_free(&err);
            return nullptr;
        }

        DBusMessage* msg = dbus_message_new_method_call("org.freedesktop.portal.Desktop",
                                                        "/org/freedesktop/portal/desktop",
                                                        "org.freedesktop.portal.Settings",
                                                        "Read");
        if(!msg) return nullptr;

        const char* ns = "org.freedesktop.appearance";
        dbus_message_append_args(msg, DBUS_TYPE_STRING, &ns, DBUS_TYPE_STRING, &key, DBUS_TYPE_INVALID);

        DBusMessage* reply = dbus_connection_send_with_reply_and_block(signalConn, msg, 1000, &err);
        dbus_message_unref(msg);

        if(dbus_error_is_set(&err) || !reply) {
            dbus_error_free(&err);
            return nullptr;
        }

        return reply;
    }
    IsDarkModeCommand::IsDarkModeCommand(DebusHandler* handler)
      : DbusCommand(handler) {}

    bool IsDarkModeCommand::getResult() const {
        return result;
    }

    void IsDarkModeCommand::execute_() {
        auto reply = handler->getAppearance("color-scheme");
        if(!reply) {
            result = false;
            return;
        }

        DBusMessageIter iter, variant1, variant2;
        dbus_message_iter_init(reply, &iter);

        // Unwrap outer variant
        if(dbus_message_iter_get_arg_type(&iter) != DBUS_TYPE_VARIANT) {
            dbus_message_unref(reply);
            result = false;
            return;
        }
        dbus_message_iter_recurse(&iter, &variant1);

        // Unwrap inner variant
        if(dbus_message_iter_get_arg_type(&variant1) != DBUS_TYPE_VARIANT) {
            dbus_message_unref(reply);
            result = false;
            return;
        }
        dbus_message_iter_recurse(&variant1, &variant2);

        if(dbus_message_iter_get_arg_type(&variant2) != DBUS_TYPE_UINT32) {
            dbus_message_unref(reply);
            result = false;
            return;
        }

        uint32_t colorScheme;
        dbus_message_iter_get_basic(&variant2, &colorScheme);
        dbus_message_unref(reply);

        // Per portal spec: 0=no preference, 1=dark, 2=light
        result = (colorScheme == 1);
    }
    GetAccentColorCommand::GetAccentColorCommand(DebusHandler* handler)
      : DbusCommand(handler) {}

    ThemeColors GetAccentColorCommand::getResult() const {
        return result;
    }

    void GetAccentColorCommand::execute_() {
        auto reply = handler->getAppearance("accent-color");
        if(!reply) {
            result = {53, 132, 228};  // default blue accent
            return;
        }

        // Reply is v(v(r,g,b)) — outer variant wraps inner variant of struct (ddd)
        DBusMessageIter iter, variant1, variant2, struct_iter;
        dbus_message_iter_init(reply, &iter);
        if(dbus_message_iter_get_arg_type(&iter) != DBUS_TYPE_VARIANT) {
            dbus_message_unref(reply);
            result = {53, 132, 228};
            return;
        }
        dbus_message_iter_recurse(&iter, &variant1);
        if(dbus_message_iter_get_arg_type(&variant1) != DBUS_TYPE_VARIANT) {
            dbus_message_unref(reply);
            result = {53, 132, 228};
            return;
        }
        dbus_message_iter_recurse(&variant1, &variant2);
        if(dbus_message_iter_get_arg_type(&variant2) != DBUS_TYPE_STRUCT) {
            dbus_message_unref(reply);
            result = {53, 132, 228};
            return;
        }
        dbus_message_iter_recurse(&variant2, &struct_iter);

        double r = 0, g = 0, b = 0;
        dbus_message_iter_get_basic(&struct_iter, &r);
        dbus_message_iter_next(&struct_iter);
        dbus_message_iter_get_basic(&struct_iter, &g);
        dbus_message_iter_next(&struct_iter);
        dbus_message_iter_get_basic(&struct_iter, &b);

        dbus_message_unref(reply);

        result = {(uint8_t) (r * 255), (uint8_t) (g * 255), (uint8_t) (b * 255)};
    }

    SetThemeChangeCallbackCommand::SetThemeChangeCallbackCommand(DebusHandler* handler)
      : DbusCommand(handler) {}

    void SetThemeChangeCallbackCommand::execute_() {
        DBusError err;
        dbus_error_init(&err);
        if(!handler->signalConn) {
            handler->signalConn = dbus_bus_get(DBUS_BUS_SESSION, &err);
        }
        if(!handler->signalConn || dbus_error_is_set(&err)) {
            dbus_error_free(&err);
            return;
        }

        dbus_bus_add_match(handler->signalConn,
                           "type='signal',"
                           "interface='org.freedesktop.portal.Settings',"
                           "member='SettingChanged',"
                           "path='/org/freedesktop/portal/desktop'",
                           &err);
        dbus_connection_flush(handler->signalConn);

        if(dbus_error_is_set(&err)) {
            dbus_error_free(&err);
            return;
        }
    }

    DebusHandler::DebusHandler() {
        start();
    }
    DebusHandler::~DebusHandler() {
        if(signalConn) {
            dbus_connection_unref(signalConn);
            signalConn = nullptr;
        }
        stop();
    }

    DebusHandler& DebusHandler::getInstance() {
        static DebusHandler instance;
        return instance;
    }

    void DebusHandler::handleEvents() {
        if(!signalConn) return;
        dbus_connection_read_write(signalConn, 100);
        if(!signalConn) return; // If the connection was closed during read/write, just return.
        DBusMessage* msg = dbus_connection_pop_message(signalConn);
        if(!msg) return;

        if(dbus_message_is_signal(msg, "org.freedesktop.portal.Settings", "SettingChanged")) {
            // We got a theme change signal, but we can't do much work in this thread since it's also responsible
            // for reading DBus messages. So we just notify the worker thread to do the actual work of calling the
            // callback.

            // std::this_thread::sleep_for(std::chrono::milliseconds(500));  // Add a short delay to allow the theme to actually change before we query it
            if(themeChangeCallback) {
                themeChangeCallback();
            }
            auto& systemTheme = SystemTheme::getInstance();
            systemTheme.callCallback();
        }
        dbus_message_unref(msg);
    }

    void DebusHandler::dispatchCommand(std::shared_ptr<DbusCommand> command) {
        if(std::this_thread::get_id() == dbusThread.get_id()) {
            // Already in DBus thread, execute directly
            command->execute();
        } else {
            commandQueue.push(command);
        }
    }

    void DebusHandler::dbusThreadFunc() {
        while(dbusThreadRunning) {
            if(!commandQueue.empty()) {
                auto command = commandQueue.pop();
                if(!dbusThreadRunning || !command) break;
                command.value()->execute();
            }
            handleEvents();
        }
    }

    void DebusHandler::start() {
        dbusThreadRunning = true;
        dbusThread        = std::thread(&DebusHandler::dbusThreadFunc, this);
    }

    void DebusHandler::stop() {
        dbusThreadRunning = false;
        commandQueue.stop();
        if(dbusThread.joinable()) dbusThread.join();
    }

    ThemeColors DebusHandler::getAccentColor() {
        auto command = std::make_shared<GetAccentColorCommand>(this);
        dispatchCommand(command);
        command->waitForCompletion();
        return command->getResult();
    }

    bool DebusHandler::isDarkMode() {
        auto command = std::make_shared<IsDarkModeCommand>(this);
        dispatchCommand(command);
        command->waitForCompletion();
        return command->getResult();
    }

    void DebusHandler::setThemeChangeCallback(std::function<void()> callback) {
        themeChangeCallback = std::move(callback);
        auto command        = std::make_shared<SetThemeChangeCallbackCommand>(this);
        dispatchCommand(command);
        command->waitForCompletion();
    }
    void DebusHandler::removeThemeChangeCallback() {
        themeChangeCallback = nullptr;
    }

}  // namespace system_theme_pp