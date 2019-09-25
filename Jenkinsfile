pipeline {
  agent any
  stages {
    stage('initial_setup') {
      steps {
        sh '''cd android
        fastlane save_git_log
        fastlane gradle_clean_bridge'''
      }
    }
    stage('node_modules (clean install)') {
      steps {
        sh '''cd Examples
        rm -rf node_modules/react-viro ; npm install'''
      }
    }
    stage('viroreact (android)') {
      steps {
        sh '''cd android
        fastlane react_viro_bridge_aar'''
      }
    }
    stage('viroreact (iOS)') {
      steps {
        sh '''cd Examples/ios
        fastlane release_react_viro_lib_clean
        fastlane release_react_viro_lib
        fastlane release_react_viro_static_lib'''
      }
    }
    stage('prepare_release') {
      steps {
        sh '''rm -rf react-viro-*.tgz
        ./prepare_release.sh
        cp react-viro-*.tgz /var/tmp/build_intermediates/s3_artifacts/'''
      }
    }
    stage('release_tests(android)(gvr+ovr)') {
      steps {
        sh '''cd Examples/android/
        fastlane gvr_release_test
        fastlane ovr_release_test'''
      }
    }
    stage('upload_to_s3_slack ') {
      steps {
        sh '''cd android/
fastlane upload_builds_and_slack'''
      }
    }
  }
  environment {
    LC_ALL = 'en_US.UTF-8'
    LANG = 'en_US.UTF-8'
  }
}
