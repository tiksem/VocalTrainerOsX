#include "Executors.h"
#include "app.h"
#include <QThreadPool>

namespace CppUtils {
    namespace Executors {
        class _Runnable : public QRunnable {
            std::function<void()> function;

        public:
            _Runnable(const std::function<void()> &function) : function(function) {
            }

            void run() override {

            }
        };

        void ExecuteOnMainThread(std::function<void()> function) {
            App::instance()->executeOnMainThread(function);
        }

        void ExecuteOnBackgroundThread(std::function<void()> function) {
            _Runnable* runnable = new _Runnable(function);
            QThreadPool::globalInstance()->start(runnable);
        }
    }
}