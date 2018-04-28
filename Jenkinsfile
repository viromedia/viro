pipeline {
  agent any
  stages {
    stage('') {
      steps {
        sh '''cd Examples/android
./gradlew assembleGvrRelease'''
      }
    }
  }
  environment {
    LC_ALL = 'en_US.UTF-8'
    LANG = 'en_US.UTF-8'
    ANDROID_HOME = '/Users/Shared/Jenkins/Library/Android/sdk'
  }
}