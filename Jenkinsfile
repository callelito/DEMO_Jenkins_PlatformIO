pipeline {
  agent any
  stages {
    stage('Build') {
      environment {
        BUILD_TOKEN = credentials('BUILD_TOKEN')
      }
      steps {
        sh '''PLATFORMIO_AUTH_TOKEN=${BUILD_TOKEN} pio remote run -r
pio account logout'''
      }
    }

    stage('Test') {
      environment {
        TEST_TOKEN = credentials('TEST_TOKEN')
      }
      steps {
        sh '''PLATFORMIO_AUTH_TOKEN=${TEST_TOKEN} pio remote test -e native -r
pio account logout'''
        sh '''PLATFORMIO_AUTH_TOKEN=${TEST_TOKEN} pio remote test -r
pio account logout'''
      }
    }

  }
}
