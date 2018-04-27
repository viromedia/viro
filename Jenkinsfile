pipeline {
  agent {
    node {
      label 'react-viro'
    }

  }
  stages {
    stage('Code Checkout') {
      steps {
        sh 'https://github.com/viromedia/ViroRenderer.git'
        sh 'https://github.com/viromedia/react-viro.git'
      }
    }
  }
}