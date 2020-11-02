require "json"

package = JSON.parse(File.read(File.join(__dir__, "package.json")))
file = File.new("ios/quickjs/VERSION", chomp: true)
version = file.readlines[0].chomp

folly_compiler_flags = '-DFOLLY_NO_CONFIG -DFOLLY_MOBILE=1 -DFOLLY_USE_LIBCPP=1 -Wno-comma -Wno-shorten-64-to-32'
folly_version = '2018.10.22.00'
boost_compiler_flags = '-Wno-documentation'

Pod::Spec.new do |s|
  s.name         = "react-native-quickjs"
  s.version      = package["version"]
  s.summary      = package["description"]
  s.homepage     = package["homepage"]
  s.license      = package["license"]
  s.authors      = package["author"]

  s.platforms    = { :ios => "9.0" }
  s.compiler_flags         = folly_compiler_flags + ' ' + boost_compiler_flags
  s.source       = { :http => "https://github.com/onesmash/react-native-quickjs.git" }
  s.source_files = "ios/**/*.{h,c,cpp,mm}"
  s.exclude_files = "ios/quickjs/qjsc.c", "ios/quickjs/qjs.c", "ios/quickjs/run-test262.c", "ios/quickjs-2020-07-05/unicode_gen.c", "ios/quickjs/doc/**/*", "ios/quickjs/examples/**/*", "ios/quickjs/tests/**/*"
  #s.library                = "stdc++"
  s.pod_target_xcconfig    = { "HEADER_SEARCH_PATHS" => "\"$(PODS_ROOT)/boost-for-react-native\" \"$(PODS_ROOT)/Folly\"" }
  s.user_target_xcconfig    = {
    "CLANG_CXX_LANGUAGE_STANDARD" => "c++14",
    "HEADER_SEARCH_PATHS" => "\"$(PODS_ROOT)/boost-for-react-native\" \"$(PODS_ROOT)/Folly\""
  }
  s.xcconfig = { "OTHER_CFLAGS" => <<-DESC
    $(inherited) -DCONFIG_VERSION="\\"#{version}\\"" #{folly_compiler_flags} #{boost_compiler_flags}
    DESC
    }
  s.dependency 'React'
  s.dependency "Folly", folly_version

end
