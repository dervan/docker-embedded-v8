#include <v8.h>
#include <libplatform/libplatform.h>
#include <iostream>

namespace {

double GetResult(v8::Local<v8::Context> ctx, v8::Local<v8::Value> val) {
  v8::Local<v8::Number> result_num = val->ToNumber(ctx).ToLocalChecked();
  return result_num->Value();
}

v8::Local<v8::Script> CompileLatinStr(v8::Local<v8::Context> ctx, const std::string &str) {
  v8::Local<v8::String> script_str = v8::String::NewFromOneByte(ctx->GetIsolate(),
                                                                reinterpret_cast<const uint8_t *>(str.c_str())).ToLocalChecked();
  v8::Local<v8::Script> script = v8::Script::Compile(ctx, script_str).ToLocalChecked();
  return script;
}

std::unique_ptr<v8::Platform> InitOnce(const std::string &exec_location) {
  v8::V8::InitializeICUDefaultLocation(exec_location.c_str());
  v8::V8::InitializeExternalStartupData(exec_location.c_str());
  std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
  v8::V8::InitializePlatform(platform.get());
  v8::V8::Initialize();
  return platform;
}

double RunScriptGetResult(const std::string js_script_txt, v8::Isolate *isolate) {
  v8::Isolate::Scope isolate_scope(isolate);
  v8::HandleScope handle_scope(isolate);
  v8::Local<v8::Context> context = v8::Context::New(isolate);
  v8::Context::Scope context_scope(context);
  v8::Local<v8::Script> script = CompileLatinStr(context, js_script_txt);
  return GetResult(context, script->Run(context).ToLocalChecked());
}

v8::Isolate *CreateIsolate(v8::StartupData *snapshot) {
  v8::Isolate::CreateParams create_params;
  create_params.array_buffer_allocator_shared = std::shared_ptr<v8::ArrayBuffer::Allocator>(
          v8::ArrayBuffer::Allocator::NewDefaultAllocator());
  create_params.snapshot_blob = snapshot;
  return v8::Isolate::New(create_params);
}

std::string StdinToString() {
  std::string input_str;
  while (!std::cin.eof()) {
    std::vector<char> input_part(1024);
    std::cin.read(input_part.data(), input_part.size());
    input_str += std::string(input_part.data(), input_part.data() + std::cin.gcount());
  }
  return input_str;
}

}

int main(int argc, char *argv[]) {
  std::string test_fn = StdinToString();
  int num_internal_iterations = 2000;
  int num_external_iterations = 42;

  std::string eval_loop = "test_fn = " + test_fn +  R"js(
  function evaluate() {
    let r = 0;
    for (let j = 0; j < )js" + std::to_string(num_internal_iterations) + R"js(; j++) {
      r += test_fn();
    }
    return r;
  }
  evaluate();
  )js";

  auto platform = InitOnce(argv[0]);
  v8::Isolate *isolate = CreateIsolate(nullptr);

  double r = 0.;
  for (size_t i = 0; i < num_external_iterations; ++i) {
    r += RunScriptGetResult(eval_loop, isolate);
  }
  
  std::cout << "Sum of results: " << r << std::endl;

  isolate->Dispose();
  v8::V8::Dispose();
  v8::V8::ShutdownPlatform();

  return 0;
}
