require 'json'

package = JSON.parse(File.read(File.join(__dir__, 'package.json')))

Pod::Spec.new do |s|
  s.name         = "react-viro"
  s.version      = package['version']
  s.summary      = "React Viro"

  s.author       = 'Viro Media'
  s.homepage     = 'http://viromedia.com'
  s.license      = "MIT"
  s.platform     = :ios, "8.0"

  s.source       = { :git => "https://github.com/mendix/viro.git", :tag=>"head" :branch=> "nalm/auto-linking" }
  s.source_files  = ["ios/ViroReact/**/*.{h,m}", "ios/ViroReactFramework/**/*.{h,m}"]

  s.dependency 'React-Core'
  s.dependency 'GoogleMaps', '3.5.0'
  s.dependency 'Google-Maps-iOS-Utils', '2.1.0'
end