require 'json'

package = JSON.parse(File.read(File.join(__dir__, '../package.json')))

Pod::Spec.new do |s|
  s.name                = 'ViroReact'
  s.version             = '1.0'
  s.summary             = 'Test description'
  s.source              = { :http => 'https://s3-us-west-2.amazonaws.com/viro-docs/ViroReact.tar.gz'}
  s.source_files        = 'dist/include'
  s.public_header_files = 'dist/include/*.h'
  s.vendored_libraries  = 'dist/lib/libViroReact.a'
  s.homepage            = 'http://viromedia.com'
  s.license             = {:type => 'Copyright', :text => "Copyright 2018 ViroMedia, Inc" }
  s.author              = 'Viro Media'
  s.requires_arc        = true
  s.platform            = :ios, '9.3'
end
