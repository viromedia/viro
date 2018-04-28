pipeline {
  agent any
  stages {
    stage('Code Checkout') {
      steps {
        sh 'https://github.com/viromedia/ViroRenderer.git'
        sh 'https://github.com/viromedia/react-viro.git'
      }
    }
  }
  environment {
    LC_ALL = 'en_US.UTF-8'
    LANG = 'en_US.UTF-8'
    ANDROID_HOME = '/Users/Shared/Jenkins/Library/Android/sdk'
  }
}