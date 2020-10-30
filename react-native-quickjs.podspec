require "json"

package = JSON.parse(File.read(File.join(__dir__, "package.json")))
file = File.new("ios/quickjs/VERSION", chomp: true)
version = file.readlines[0].chomp

Pod::Spec.new do |s|
  s.name         = "react-native-quickjs"
  s.version      = package["version"]
  s.summary      = package["description"]
  s.homepage     = package["homepage"]
  s.license      = package["license"]
  s.authors      = package["author"]

  s.platforms    = { :ios => "9.0" }
  s.source       = { :http => "https://github.com/onesmash/react-native-quickjs.git" }
  s.source_files = "ios/**/*.{h,c,cpp,mm}"
  s.exclude_files = "ios/quickjs/qjsc.c", "ios/quickjs/qjs.c", "ios/quickjs/run-test262.c", "ios/quickjs-2020-07-05/unicode_gen.c", "ios/quickjs/doc/**/*", "ios/quickjs/examples/**/*", "ios/quickjs/tests/**/*"
  s.xcconfig = { "OTHER_CFLAGS" => <<-DESC
    $(inherited) -DCONFIG_VERSION="\\"#{version}\\""
    DESC
    }
  s.dependency 'React'
  #s.dependency 'React-Core'

end
