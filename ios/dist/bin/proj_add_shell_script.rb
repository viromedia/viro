require 'xcodeproj'
project_name = ARGV[0]
path_to_project = "#{project_name}.xcodeproj"
project = Xcodeproj::Project.open(path_to_project)
main_target = project.targets.first
phase = main_target.new_shell_script_build_phase("Execute Ngrok")
phase.shell_script = "$SRCROOT/ngrok_launch.sh"


phaseTwo = main_target.new_shell_script_build_phase("Copy ngrok endpoint")
phaseTwo.shell_script = "DEST=$CONFIGURATION_BUILD_DIR/$UNLOCALIZED_RESOURCES_FOLDER_PATH\nendpointfile=\"ngrokendpoint.txt\"\nif [[ -e \"$SRCROOT/$endpointfile\" ]]; then\necho \"Ngrok endpoint file found, copying over...\"\ncp \"$SRCROOT/$endpointfile\" \"$DEST/$endpointfile\"\nbreak;\nfi"

project.targets.each do |target|
  target.build_configurations.each do |config|
    config.build_settings['ENABLE_BITCODE'] = 'NO'
    config.build_settings['DEAD_CODE_STRIPPING'] = 'YES'
  end
end


project.save()
