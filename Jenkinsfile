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

    stage('Test') {
      steps {
        sh '''PLATFORMIO_AUTH_TOKEN=${TEST_TOKEN} pio remote test -e native -r
pio account logout'''
        sh '''PLATFORMIO_AUTH_TOKEN=${TEST_TOKEN} pio remote test -r
pio account logout'''
      }
    }

  }
  environment {
    BUILD_TOKEN = credentials('TEST_TOKEN')
    TEST_TOKEN = credentials('TEST_TOKEN')
  }
}