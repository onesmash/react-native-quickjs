//
//  QJSRuntime.cpp
//  react-native-quickjs
//
//  Created by 徐晖 on 2020/9/14.
//

#include "QJSRuntime.h"
#include <sstream>
#include <jsi/jsilib.h>
#include <quickjs-libc.h>
using namespace facebook;
using namespace jsi;

namespace qjs {

namespace {
std::string JSStringToSTLString(JSContext *ctx, JSValue str) {
    const char* val = JS_ToCString(ctx, str);
    auto res = std::string(val);
    JS_FreeCString(ctx, val);
    return res;
}

//JSStringRef getLengthString() {
//  static JSStringRef length = JSStringCreateWithUTF8CString("length");
//  return length;
//}
//
//JSStringRef getNameString() {
//  static JSStringRef name = JSStringCreateWithUTF8CString("name");
//  return name;
//}
//
//JSStringRef getFunctionString() {
//  static JSStringRef func = JSStringCreateWithUTF8CString("Function");
//  return func;
//}
//
//#if !defined(_JSC_FAST_IS_ARRAY)
//JSStringRef getArrayString() {
//  static JSStringRef array = JSStringCreateWithUTF8CString("Array");
//  return array;
//}
//
//JSStringRef getIsArrayString() {
//  static JSStringRef isArray = JSStringCreateWithUTF8CString("isArray");
//  return isArray;
//}
//#endif
}

// std::string utility
namespace {
std::string to_string(void* value) {
  std::ostringstream ss;
  ss << value;
  return ss.str();
}
} // namespace

//typedef struct JSValue *JSValueRef;

class QJSRuntime : public jsi::Runtime {
public:
    QJSRuntime();
    QJSRuntime(JSContext *ctx);
    
    ~QJSRuntime();
    
    facebook::jsi::Value evaluateJavaScript(const std::shared_ptr<const Buffer> &buffer, const std::string &sourceURL) override;
    
    std::shared_ptr<const PreparedJavaScript> prepareJavaScript(const std::shared_ptr<const Buffer> &buffer, std::string sourceURL) override;
    
    facebook::jsi::Value evaluatePreparedJavaScript(const std::shared_ptr<const PreparedJavaScript> &js) override;
    
    facebook::jsi::Object global() override;
    
    std::string description() override;
    
    bool isInspectable() override;
    
    facebook::jsi::Runtime::PointerValue *cloneSymbol(const Runtime::PointerValue *pv) override;
    
    facebook::jsi::Runtime::PointerValue *cloneString(const Runtime::PointerValue *pv) override;
    
    facebook::jsi::Runtime::PointerValue *cloneObject(const Runtime::PointerValue *pv) override;
    
    facebook::jsi::Runtime::PointerValue *clonePropNameID(const Runtime::PointerValue *pv) override;
    
    facebook::jsi::PropNameID createPropNameIDFromAscii(const char *str, size_t length) override;
    
    facebook::jsi::PropNameID createPropNameIDFromUtf8(const uint8_t *utf8, size_t length) override;
    
    facebook::jsi::PropNameID createPropNameIDFromString(const facebook::jsi::String &str) override;
    
    std::string utf8(const facebook::jsi::PropNameID &) override;
    
    bool compare(const facebook::jsi::PropNameID &, const facebook::jsi::PropNameID &) override;
    
    std::string symbolToString(const facebook::jsi::Symbol &) override;
    
    facebook::jsi::String createStringFromAscii(const char *str, size_t length) override;
    
    facebook::jsi::String createStringFromUtf8(const uint8_t *utf8, size_t length) override;
    
    std::string utf8(const facebook::jsi::String &) override;
    
    facebook::jsi::Object createObject() override;
    
    facebook::jsi::Object createObject(std::shared_ptr<HostObject> ho) override;
    
    std::shared_ptr<HostObject> getHostObject(const jsi::Object &) override;
    
    facebook::jsi::HostFunctionType &getHostFunction(const jsi::Function &) override;
    
    facebook::jsi::Value getProperty(const facebook::jsi::Object &, const facebook::jsi::PropNameID &name) override;
    
    facebook::jsi::Value getProperty(const facebook::jsi::Object &, const facebook::jsi::String &name) override;
    
    bool hasProperty(const facebook::jsi::Object &, const facebook::jsi::PropNameID &name) override;
    
    bool hasProperty(const facebook::jsi::Object &, const facebook::jsi::String &name) override;
    
    void setPropertyValue(facebook::jsi::Object &, const facebook::jsi::PropNameID &name, const facebook::jsi::Value &value) override;
    
    void setPropertyValue(facebook::jsi::Object &, const facebook::jsi::String &name, const facebook::jsi::Value &value) override;
    
    bool isArray(const facebook::jsi::Object &) const override;
    
    bool isArrayBuffer(const facebook::jsi::Object &) const override;
    
    bool isFunction(const facebook::jsi::Object &) const override;
    
    bool isHostObject(const jsi::Object &) const override;
    
    bool isHostFunction(const jsi::Function &) const override;
    
    facebook::jsi::Array getPropertyNames(const facebook::jsi::Object &) override;
    
    facebook::jsi::WeakObject createWeakObject(const facebook::jsi::Object &) override;
    
    facebook::jsi::Value lockWeakObject(const facebook::jsi::WeakObject &) override;
    
    facebook::jsi::Array createArray(size_t length) override;
    
    size_t size(const facebook::jsi::Array &) override;
    
    size_t size(const facebook::jsi::ArrayBuffer &) override;
    
    uint8_t *data(const facebook::jsi::ArrayBuffer &) override;
    
    facebook::jsi::Value getValueAtIndex(const facebook::jsi::Array &, size_t i) override;
    
    void setValueAtIndexImpl(facebook::jsi::Array &, size_t i, const facebook::jsi::Value &value) override;
    
    facebook::jsi::Function createFunctionFromHostFunction(const facebook::jsi::PropNameID &name, unsigned int paramCount, facebook::jsi::HostFunctionType func) override;
    
    facebook::jsi::Value call(const facebook::jsi::Function &, const facebook::jsi::Value &jsThis, const facebook::jsi::Value *args, size_t count) override;
    
    facebook::jsi::Value callAsConstructor(const facebook::jsi::Function &, const facebook::jsi::Value *args, size_t count) override;
    
    bool strictEquals(const facebook::jsi::Symbol &a, const facebook::jsi::Symbol &b) const override;
    
    bool strictEquals(const facebook::jsi::String &a, const facebook::jsi::String &b) const override;
    
    bool strictEquals(const facebook::jsi::Object &a, const facebook::jsi::Object &b) const override;
    
    bool instanceOf(const facebook::jsi::Object &o, const facebook::jsi::Function &f) override;
    
    jsi::Value createValue(JSValue value) const;
    
    JSValue valueRef(const jsi::Value& value);
    
protected:
    class QJSSymbolValue final : public PointerValue {
    #ifndef NDEBUG
        QJSSymbolValue(JSContext *ctx_,
                       const std::atomic<bool>& ctxInvalid,
                       JSValue sym, std::atomic<intptr_t>& counter);
    #else
        QJSSymbolValue(JSContext *ctx_,
                       const std::atomic<bool>& ctxInvalid,
                       JSValue sym);
    #endif
        void invalidate() override;

        JSContext *ctx_;
        const std::atomic<bool>& ctxInvalid_;
        // There is no C type in the JSC API to represent Symbol, so this stored
        // a JSValueRef which contains the Symbol.
        JSValue sym_;
    #ifndef NDEBUG
        std::atomic<intptr_t>& counter_;
    #endif
       protected:
        friend class QJSRuntime;
      };
    
    class QJSStringValue final : public PointerValue {
    #ifndef NDEBUG
        QJSStringValue(JSContext *ctx_, const std::atomic<bool>& ctxInvalid, JSValue str, std::atomic<intptr_t>& counter);
    #else
        QJSStringValue(JSContext *ctx_, const std::atomic<bool>& ctxInvalid, JSValue str);
    #endif
        void invalidate() override;

        const std::atomic<bool>& ctxInvalid_;
        JSContext *ctx_;
        JSValue str_;
    #ifndef NDEBUG
        std::atomic<intptr_t>& counter_;
    #endif
       protected:
        friend class QJSRuntime;
      };
    
    class QJSObjectValue final : public PointerValue {
        QJSObjectValue(
            JSContext *ctx_,
            const std::atomic<bool>& ctxInvalid,
            JSValue obj
    #ifndef NDEBUG
            ,
            std::atomic<intptr_t>& counter
    #endif
                       );

        void invalidate() override;

        const std::atomic<bool>& ctxInvalid_;
        JSContext *ctx_;
        JSValue obj_;
    #ifndef NDEBUG
        std::atomic<intptr_t>& counter_;
    #endif
       protected:
        friend class QJSRuntime;
      };
    
private:
    static JSValue symbolRef(const jsi::Symbol& str);
    static JSValue stringRef(const jsi::String& str);
    static JSValue stringRef(const jsi::PropNameID& sym);
    static JSValue objectRef(const jsi::Object& obj);
    
    void checkException(JSValue exc);
    void checkException(JSValue res, JSValue exc);
    void checkException(JSValue exc, const char* msg);
    void checkException(JSValue res, JSValue exc, const char* msg);
    jsi::Symbol createSymbol(JSValue symbol) const;
    jsi::String createString(JSValue str) const;
    jsi::PropNameID createPropNameID(JSValue str);
    jsi::Object createObject(JSValue object) const;
    
    jsi::Runtime::PointerValue* makeSymbolValue(JSValue sym) const;
    jsi::Runtime::PointerValue* makeStringValue(JSValue str) const;
    jsi::Runtime::PointerValue* makeObjectValue(JSValue obj) const;
    
    JSContext *ctx_;
    std::atomic<bool> ctxInvalid_;
    std::string desc_;
#ifndef NDEBUG
    mutable std::atomic<intptr_t> objectCounter_;
    mutable std::atomic<intptr_t> symbolCounter_;
    mutable std::atomic<intptr_t> stringCounter_;
#endif
    
};

QJSRuntime::QJSRuntime(): QJSRuntime(JS_NewContext(JS_NewRuntime())) {
    JS_FreeContext(ctx_);
}

QJSRuntime::QJSRuntime(JSContext *ctx)
    : ctx_(JS_DupContext(ctx)),
      ctxInvalid_(false)
#ifndef NDEBUG
      ,
      objectCounter_(0),
      stringCounter_(0)
#endif
{
}

QJSRuntime::~QJSRuntime() {
    ctxInvalid_ = true;
    JS_FreeContext(ctx_);
#ifndef NDEBUG
    assert(
      objectCounter_ == 0 && "QJSRuntime destroyed with a dangling API object");
    assert(
      stringCounter_ == 0 && "QJSRuntime destroyed with a dangling API string");
#endif
}

jsi::Runtime::PointerValue* QJSRuntime::makeSymbolValue(
    JSValue symbol) const {
#ifndef NDEBUG
  return new QJSSymbolValue(ctx_, ctxInvalid_, symbol, symbolCounter_);
#else
  return new QJSSymbolValue(ctx_, ctxInvalid_, symbol);
#endif
}

jsi::Runtime::PointerValue* QJSRuntime::makeStringValue(
    JSValue str) const {
#ifndef NDEBUG
  return new QJSStringValue(ctx_, ctxInvalid_, str, stringCounter_);
#else
  return new QJSStringValue(ctx_, ctxInvalid_, str);
#endif
}

jsi::Runtime::PointerValue* QJSRuntime::makeObjectValue(
    JSValue object) const {
#ifndef NDEBUG
  return new QJSObjectValue(ctx_, ctxInvalid_, object, objectCounter_);
#else
  return new QJSObjectValue(ctx_, ctxInvalid_, object);
#endif
}


jsi::Symbol QJSRuntime::createSymbol(JSValue sym) const {
  return make<jsi::Symbol>(makeSymbolValue(sym));
}

jsi::String QJSRuntime::createString(JSValue str) const {
  return make<jsi::String>(makeStringValue(str));
}

jsi::PropNameID QJSRuntime::createPropNameID(JSValue str) {
  return make<jsi::PropNameID>(makeStringValue(str));
}

jsi::Object QJSRuntime::createObject(JSValue obj) const {
  return make<jsi::Object>(makeObjectValue(obj));
}

jsi::Value QJSRuntime::createValue(JSValue value) const {
  if (JS_IsNumber(value)) {
      double number;
      JS_ToFloat64(ctx_, &number, value);
      return jsi::Value(number);
  } else if (JS_IsBool(value)) {
      bool res = JS_ToBool(ctx_, value);
      return jsi::Value(res);
  } else if (JS_IsNull(value)) {
      return jsi::Value(nullptr);
  } else if (JS_IsUndefined(value)) {
      return jsi::Value();
  } else if (JS_IsString(value)) {
      return jsi::Value(createString(value));
  } else if (JS_IsObject(value)) {
    return jsi::Value(createObject(value));
  } else if (JS_IsSymbol(value)) {
    return jsi::Value(createSymbol(value));
  } else {
    // WHAT ARE YOU
    abort();
  }
}

JSValue QJSRuntime::valueRef(const jsi::Value& value) {
  // I would rather switch on value.kind_
  if (value.isUndefined()) {
    return JS_UNDEFINED;
  } else if (value.isNull()) {
    return JS_NULL;
  } else if (value.isBool()) {
      return JS_NewBool(ctx_, value.getBool());
  } else if (value.isNumber()) {
      return JS_NewFloat64(ctx_, value.getNumber());
  } else if (value.isSymbol()) {
    return symbolRef(value.getSymbol(*this));
  } else if (value.isString()) {
    return stringRef(value.getString(*this));
  } else if (value.isObject()) {
    return objectRef(value.getObject(*this));
  } else {
    // What are you?
    abort();
  }
}

JSValue QJSRuntime::symbolRef(const jsi::Symbol& sym) {
  return static_cast<const QJSSymbolValue*>(getPointerValue(sym))->sym_;
}

JSValue QJSRuntime::stringRef(const jsi::String& str) {
  return static_cast<const QJSStringValue*>(getPointerValue(str))->str_;
}

JSValue QJSRuntime::stringRef(const jsi::PropNameID& sym) {
  return static_cast<const QJSStringValue*>(getPointerValue(sym))->str_;
}

JSValue QJSRuntime::objectRef(const jsi::Object& obj) {
  return static_cast<const QJSObjectValue*>(getPointerValue(obj))->obj_;
}


std::shared_ptr<const jsi::PreparedJavaScript> QJSRuntime::prepareJavaScript(
    const std::shared_ptr<const jsi::Buffer> &buffer,
    std::string sourceURL) {
  return std::make_shared<jsi::SourceJavaScriptPreparation>(
      buffer, std::move(sourceURL));
}

jsi::Value QJSRuntime::evaluatePreparedJavaScript(
const std::shared_ptr<const jsi::PreparedJavaScript>& js) {
    assert(
      dynamic_cast<const jsi::SourceJavaScriptPreparation*>(js.get()) &&
      "preparedJavaScript must be a SourceJavaScriptPreparation");
    auto sourceJs =
        std::static_pointer_cast<const jsi::SourceJavaScriptPreparation>(js);
    std::string tmp(
                    reinterpret_cast<const char*>(sourceJs->data()), sourceJs->size());
    JSValue obj, val;
    obj = JS_ReadObject(ctx_, (uint8_t*)tmp.c_str(), tmp.size(), JS_READ_OBJ_BYTECODE);
    checkException(obj);
    if (JS_VALUE_GET_TAG(obj) == JS_TAG_MODULE) {
        if (JS_ResolveModule(ctx_, obj) < 0) {
            JS_FreeValue(ctx_, obj);
            goto exception;
        }
        js_module_set_import_meta(ctx_, obj, 0, 1);
    }
    val = JS_EvalFunction(ctx_, obj);
    if (JS_IsException(val)) {
    exception:
        js_std_dump_error(ctx_);
        exit(1);
    }
    auto res = createValue(val);
    JS_FreeValue(ctx_, val);
    return res;
}

jsi::Value QJSRuntime::evaluateJavaScript(
    const std::shared_ptr<const jsi::Buffer> &buffer,
    const std::string& sourceURL) {
    std::string tmp(
                    reinterpret_cast<const char*>(buffer->data()), buffer->size());
    JSValue val = JS_Eval(ctx_, tmp.c_str(), tmp.size(), sourceURL.c_str(), 0);
    checkException(val);
    auto res = createValue(val);
    JS_FreeValue(ctx_, val);
    return res;
}

jsi::Object QJSRuntime::global() {
    JSValue globalObj = JS_GetGlobalObject(ctx_);
    auto res = createObject(globalObj);
    JS_FreeValue(ctx_, globalObj);
    return res;
}

std::string QJSRuntime::description() {
  if (desc_.empty()) {
    desc_ = std::string("<QJSRuntime@") + to_string(this) + ">";
  }
  return desc_;
}

bool QJSRuntime::isInspectable() {
  return false;
}

QJSRuntime::QJSSymbolValue::QJSSymbolValue(
    JSContext *ctx,
    const std::atomic<bool>& ctxInvalid,
    JSValue sym
#ifndef NDEBUG
    ,
    std::atomic<intptr_t>& counter
#endif
    )
    : ctx_(ctx),
      ctxInvalid_(ctxInvalid),
      sym_(JS_DupValue(ctx, sym))
#ifndef NDEBUG
      ,
      counter_(counter)
#endif
{
#ifndef NDEBUG
  counter_ += 1;
#endif
}

void QJSRuntime::QJSSymbolValue::invalidate() {
#ifndef NDEBUG
  counter_ -= 1;
#endif

  if (!ctxInvalid_) {
      JS_FreeValue(ctx_, sym_);
  }
  delete this;
}

#ifndef NDEBUG
QJSRuntime::QJSStringValue::QJSStringValue(
    JSContext *ctx,
    const std::atomic<bool>& ctxInvalid,
    JSValue str,
    std::atomic<intptr_t>& counter)
    : ctx_(ctx), ctxInvalid_(ctxInvalid), str_(JS_DupValue(ctx_, str)), counter_(counter) {
  counter_ += 1;
}
#else
QJSRuntime::QJSStringValue::QJSStringValue(JSContext *ctx, const std::atomic<bool>& ctxInvalid, JSValue str)
    : ctx_(ctx), ctxInvalid_(ctxInvalid), str_(JS_DupValue(ctx_, *str)) {
}
#endif

void QJSRuntime::QJSStringValue::invalidate() {
#ifndef NDEBUG
  counter_ -= 1;
#endif
    if (!ctxInvalid_) {
        JS_FreeValue(ctx_, str_);
    }
    delete this;
}

QJSRuntime::QJSObjectValue::QJSObjectValue(
    JSContext *ctx,
    const std::atomic<bool>& ctxInvalid,
    JSValue obj
#ifndef NDEBUG
    ,
    std::atomic<intptr_t>& counter
#endif
    )
    : ctx_(ctx),
      ctxInvalid_(ctxInvalid),
      obj_(JS_DupValue(ctx, obj))
#ifndef NDEBUG
      ,
      counter_(counter)
#endif
{
#ifndef NDEBUG
    counter_ += 1;
#endif
}

void QJSRuntime::QJSObjectValue::invalidate() {
#ifndef NDEBUG
    counter_ -= 1;
#endif
    if (!ctxInvalid_) {
        JS_FreeValue(ctx_, obj_);
    }
    delete this;
}

jsi::Runtime::PointerValue* QJSRuntime::cloneSymbol(
    const jsi::Runtime::PointerValue* pv) {
    if (!pv) {
        return nullptr;
        
    }
    const QJSSymbolValue* symbol = static_cast<const QJSSymbolValue*>(pv);
    return makeSymbolValue(symbol->sym_);
}

jsi::Runtime::PointerValue* QJSRuntime::cloneString(
    const jsi::Runtime::PointerValue* pv) {
  if (!pv) {
    return nullptr;
  }
  const QJSStringValue* string = static_cast<const QJSStringValue*>(pv);
  return makeStringValue(string->str_);
}

jsi::Runtime::PointerValue* QJSRuntime::cloneObject(
    const jsi::Runtime::PointerValue* pv) {
  if (!pv) {
    return nullptr;
  }
  const QJSObjectValue* object = static_cast<const QJSObjectValue*>(pv);
  assert(
      object->ctx_ == ctx_ &&
      "Don't try to clone an object backed by a different Runtime");
  return makeObjectValue(object->obj_);
}

jsi::Runtime::PointerValue* QJSRuntime::clonePropNameID(
    const jsi::Runtime::PointerValue* pv) {
  if (!pv) {
    return nullptr;
  }
  const QJSStringValue* string = static_cast<const QJSStringValue*>(pv);
  return makeStringValue(string->str_);
}

jsi::PropNameID QJSRuntime::createPropNameIDFromAscii(
    const char* str,
    size_t length) {
    JSValue val = JS_NewStringLen(ctx_, str, length);
    auto res = createPropNameID(val);
    JS_FreeValue(ctx_, val);
    return res;
}

jsi::PropNameID QJSRuntime::createPropNameIDFromUtf8(
    const uint8_t* utf8,
    size_t length) {
  std::string tmp(reinterpret_cast<const char*>(utf8), length);
  JSValue val = JS_NewStringLen(ctx_, (char*)utf8, length);
  auto res = createPropNameID(val);
  JS_FreeValue(ctx_, val);
  return res;
}

jsi::PropNameID QJSRuntime::createPropNameIDFromString(const jsi::String& str) {
  return createPropNameID(stringRef(str));
}

std::string QJSRuntime::utf8(const jsi::PropNameID& sym) {
    
  return JSStringToSTLString(ctx_, stringRef(sym));
}

bool QJSRuntime::compare(const jsi::PropNameID& a, const jsi::PropNameID& b) {
    const JSString* strA = JS_VALUE_GET_STRING(stringRef(a));
    const JSString* strB = JS_VALUE_GET_STRING(stringRef(b));
    int res, len;
    len = min_int(p1->len, p2->len);
    res = js_string_memcmp(p1, p2, len);
    if (res == 0) {
        if (p1->len == p2->len)
            res = 0;
        else if (p1->len < p2->len)
            res = -1;
        else
            res = 1;
    }
    return res;
  return JSStringIsEqual(stringRef(a), stringRef(b));
}


void QJSRuntime::checkException(JSValue exc) {
  if (JS_IsException(exc)) {
      js_std_dump_error(ctx_);
      throw jsi::JSError(*this, createValue(exc));
  }
}

void QJSRuntime::checkException(JSValue res, JSValue exc) {
  if (JS_IsException(res)) {
      js_std_dump_error(ctx_);
      throw jsi::JSError(*this, createValue(exc));
  }
}

void QJSRuntime::checkException(JSValue exc, const char* msg) {
  if (JS_IsException(exc)) {
      js_std_dump_error(ctx_);
      throw jsi::JSError(std::string(msg), *this, createValue(exc));
  }
}

void QJSRuntime::checkException(
    JSValue res,
    JSValue exc,
    const char* msg) {
  if (JS_IsException(res)) {
      js_std_dump_error(ctx_);
      throw jsi::JSError(std::string(msg), *this, createValue(exc));
  }
}


std::unique_ptr<jsi::Runtime> makeJSCRuntime() {
  return std::make_unique<QJSRuntime>();
}
}
