//
//  QJSExecutorFactory.h
//  react-native-quickjs
//
//  Created by 徐晖 on 2020/10/29.
//

#ifndef QJSExecutorFactory_h
#define QJSExecutorFactory_h

#include <jsireact/JSIExecutor.h>
using namespace facebook;
using namespace react;

namespace qjs {

class QJSExecutorFactory : public JSExecutorFactory {
public:
  explicit QJSExecutorFactory(
      JSIExecutor::RuntimeInstaller runtimeInstaller)
      : runtimeInstaller_(std::move(runtimeInstaller)) {}

  std::unique_ptr<JSExecutor> createJSExecutor(
      std::shared_ptr<ExecutorDelegate> delegate,
      std::shared_ptr<MessageQueueThread> jsQueue) override;

private:
  JSIExecutor::RuntimeInstaller runtimeInstaller_;
};
}

#endif /* QJSExecutorFactory_h */
