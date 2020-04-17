pipeline {
  agent any
  environment {
        BUILD_TOKEN = credentials('BUILD_TOKEN')
    }
  stages {
    stage('Build') {
      steps {
        sh '''PLATFORMIO_AUTH_TOKEN=${BUILD_TOKEN} pio remote run -r
pio account logout'''
      }
    }

  }
}
