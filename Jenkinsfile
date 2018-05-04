pipeline {
  agent any
  stages {
    stage('clean') {
      steps {
        sh '''cd android
fastlane gradle_clean_bridge'''
      }
    }
    stage('examples_clean') {
      steps {
        sh '''cd Examples/android
fastlane examples_gradle_clean'''
      }
    }
    stage('viroreact_lib') {
      steps {
        sh '''cd android
fastlane release_react_bridge_jenkins'''
      }
    }
    stage('npm_pack') {
      steps {
        sh '''cd Examples/android
fastlane npm_pack'''
      }
    }
    stage('release_tests (gvr+ovr)') {
      steps {
        sh '''cd Examples/android/
fastlane gvr_release_test
fastlane ovr_release_test
'''
      }
    }
  }
  environment {
    LC_ALL = 'en_US.UTF-8'
    LANG = 'en_US.UTF-8'
    ANDROID_HOME = '/Users/Shared/Jenkins/Library/Android/sdk'
  }
}