pipeline {
  agent any
  stages {
    stage('Source') {
      agent any
      steps {
        git(url: 'https://github.com/ParagonGA4th/GAFinalProject', branch: 'main')
      }
    }

    stage('Build') {
      steps {
        tool 'msbuild'
      }
    }

    stage('Finished') {
      parallel {
        stage('Success') {
          steps {
            echo 'Success!'
          }
        }

        stage('Failed') {
          steps {
            error 'Failed!'
          }
        }

      }
    }

  }
}