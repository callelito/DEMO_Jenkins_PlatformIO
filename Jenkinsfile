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

    stage('Software test') {
      steps {
        sh '''pio account logout || true
PLATFORMIO_AUTH_TOKEN=${TEST_TOKEN} pio remote test -e native -r'''
        sh '''pio account logout || true
PLATFORMIO_AUTH_TOKEN=${TEST_TOKEN} pio remote test -r'''
      }
    }

    stage('Hardware test') {
      agent {
        label 'PlatformIO-slave'
      }
      steps {
        sh 'echo "hi"'
      }
    }

  }
  environment {
    BUILD_TOKEN = credentials('BUILD_TOKEN')
    TEST_TOKEN = credentials('TEST_TOKEN')
  }
}