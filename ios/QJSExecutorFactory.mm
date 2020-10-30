//
//  QJSExecutorFactory.cpp
//  react-native-quickjs
//
//  Created by 徐晖 on 2020/10/29.
//

#include "QJSExecutorFactory.h"
#include "QJSRuntime.h"

using namespace facebook;
using namespace react;
namespace qjs {

std::unique_ptr<JSExecutor> QJSExecutorFactory::createJSExecutor(
    std::shared_ptr<ExecutorDelegate> delegate,
    std::shared_ptr<MessageQueueThread> __unused jsQueue) {
  auto installBindings = [runtimeInstaller=runtimeInstaller_](jsi::Runtime &runtime) {
    react::Logger iosLoggingBinder = [](const std::string &message, unsigned int logLevel) {
      _RCTLogJavaScriptInternal(
        static_cast<RCTLogLevel>(logLevel),
        [NSString stringWithUTF8String:message.c_str()]);
    };
    react::bindNativeLogger(runtime, iosLoggingBinder);
    // Wrap over the original runtimeInstaller
    if (runtimeInstaller) {
      runtimeInstaller(runtime);
    }
  };
  return std::make_unique<JSIExecutor>(
      makeQJSRuntime(),
      delegate,
      JSIExecutor::defaultTimeoutInvoker,
      std::move(installBindings));
}

}
