#ifndef DCA74AD6_0EB7_4563_8322_260DF7D43BBA
#define DCA74AD6_0EB7_4563_8322_260DF7D43BBA

#include "blocking_deque.hpp"

#include <condition_variable>
#include <functional>
#include <thread>

#include <dbus/dbus.h>
#include <system_theme_pp/types.hpp>

namespace system_theme_pp {
    class DebusHandler;

    class DbusCommand {
      public:

        DbusCommand(DebusHandler* handler): handler(handler) {}
        virtual ~DbusCommand() = default;
        void execute() {
            execute_();
            signalCompletion();
        }
        void waitForCompletion() {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this] { return completed; });
        }

      protected:

        virtual void execute_() = 0;
        void         signalCompletion() {
            {
                std::lock_guard<std::mutex> lock(mtx);
                completed = true;
            }
            cv.notify_all();
        }

        std::mutex              mtx;
        std::condition_variable cv;
        bool                    completed = false;
        DebusHandler* handler;
    };

    class IsDarkModeCommand : public DbusCommand {
      public:

        IsDarkModeCommand(DebusHandler* handler);

        bool getResult() const;

      protected:

        void execute_() override;

      private:

        bool result = false;
    };

    class GetAccentColorCommand : public DbusCommand {
      public:

        GetAccentColorCommand(DebusHandler* handler);

        ThemeColors getResult() const;

      protected:

        void execute_() override;

      private:

        ThemeColors result{0, 0, 0};
    };

    class SetThemeChangeCallbackCommand : public DbusCommand {
      public:

        SetThemeChangeCallbackCommand(DebusHandler* handler);

      protected:

        void execute_() override;

    };

    class DebusHandler {
        friend class IsDarkModeCommand;
        friend class GetAccentColorCommand;
        friend class SetThemeChangeCallbackCommand;
        friend class RemoveThemeChangeCallbackCommand;

      public:

        static DebusHandler& getInstance();


        ~DebusHandler();

        void start();
        void stop();

        ThemeColors getAccentColor();
        bool        isDarkMode();

        void setThemeChangeCallback(std::function<void()> callback);
        void removeThemeChangeCallback();

      private:
        DBusMessage* getAppearance(const char* key);

        void dispatchCommand(std::shared_ptr<DbusCommand> command);

        void handleEvents();

        void dbusThreadFunc();

        DebusHandler();
        DebusHandler(const system_theme_pp::DebusHandler&) = delete;
        BlockingDeque<std::shared_ptr<DbusCommand>> commandQueue;
        std::thread                                 dbusThread;
        std::atomic<bool>                           dbusThreadRunning{false};
        DBusConnection*                             signalConn = nullptr;
        std::function<void()>                       themeChangeCallback;
    };
}  // namespace system_theme_pp
#endif /* DCA74AD6_0EB7_4563_8322_260DF7D43BBA */
