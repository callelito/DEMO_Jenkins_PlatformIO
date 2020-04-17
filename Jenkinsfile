pipeline {
  agent any
  stages {
    stage('Build') {
      steps {
        sh '''PLATFORMIO_AUTH_TOKEN={$BUILD_TOKEN} pio remote run -r
pio account logout'''
      }
    }

  }
}