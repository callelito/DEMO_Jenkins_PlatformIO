pipeline {
  agent any
  stages {
    stage('Build') {
      steps {
        sh '''pio account logout || true
PLATFORMIO_AUTH_TOKEN=${BUILD_TOKEN} pio remote run -r
'''
      }
    }

  }
  environment {
    BUILD_TOKEN = 
  }
}
