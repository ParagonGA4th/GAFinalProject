pipeline {
  agent any
  stages {
    stage('Source') {
      agent any
      steps {
        git(url: 'https://github.com/ParagonGA4th/GAFinalProject.git', poll: true)
      }
    }

    stage('Build') {
      steps {
        tool 'MSBuild'
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