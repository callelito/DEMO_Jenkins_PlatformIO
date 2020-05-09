<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/Callet91/DEMO_Jenkins_PlatformIO">
    <img src="images/jenkins+platformio.png" alt="Logo" width="500" height="200">
  </a>

  <h3 align="center">Demo: Using Jenkins and PlatformIO</h3>

  <p align="center">
    A simple demo showing how to setup a continious integration pipeline for development on embeded devices, specificly single-board microcontrollers, e.g. the Arduino family. 
    <br />
    <a href="https://www.youtube.com/watch?v=7JVxvhcz8Go"><strong>View demo in action [YouTube] »</strong></a>
    <br />
    <br />
    <a href="#how-it-was-made">Explore how it was made</a>
  </p>
</p>

## Table of Contents

* [About the Demo](#about-the-demo)
  * [Motivation](#motivation)
  * [Tools Used](#tools-used)
* [How it was made](#how-it-was-made)
  * [Setting Up the Tools](#setting-up-the-tools) 
    * [Jenkins Server](#jenkins-server)
    * [Build Server](#build-server)
    * [Test Server](#test-server)
    * [Hardware Test Server](#hardware-test-server)
  * [Pipeline Overview](#pipeline-overview)
    * [Build Step](#build-step)
    * [Software Test Step](#software-test-step)
    * [Hardware Test Step](#hardware-test-step)
    * [Deploy Step](#deploy-step)
* [License](#license)
* [Contact](#contact)
* [Acknowledgements](#acknowledgements)

## About The Demo

This demo aims to show how tools like **Jenkins** and **PlatformIO** can be easily used to setup a working continues integration pipeline, enabling both automatic unit testing and hardware testing. 

Specifically this demo shows how to setup a **Jenkins** automation server, and then using the Blueocean plugin setup a pipeline for automated testing using **PlatformIO**

### Motivation

When developing for embedded systems, tests that relate to the actual hardware are often done manually on a dedicated testrigg. This is time consuming and often creates a bottleneck in the workflow since only a small team can work on the testrigg one at a time. Automation of physical tests could hopefully reduce the time spent in the lab and to verify the code under test.

Also this greate article: [Continuous Delivery, Embedded Systems, and Simulation](https://blogs.windriver.com/wind_river_blog/2018/03/continuous-delivery-embedded-systems-and-simulation/) by Jakob Engblom
>Mike Long’s most important message is really that software development methods and tools matter, and that being “embedded” is no excuse not to work in a modern and efficient way.  

### Tools Used

* [Jenkins](https://www.jenkins.io/)
  * >Jenkins is a self-contained, open source automation server which can be used to automate all sorts of tasks related to building, testing, and delivering or deploying software.
    >
    >Jenkins can be installed through native system packages, Docker, or even run standalone by any machine with a Java Runtime Environment (JRE) installed.
    
    [Docs »](https://www.jenkins.io/doc/)
* [Blueocean Plugin](https://www.jenkins.io/projects/blueocean/)
  * A tool for building and visulizing **Jenkins** pipelines.
* [PlatformIO](https://platformio.org/)
  * >PlatformIO is a cross-platform, cross-architecture, multiple framework, professional tool for embedded systems engineers and for software developers who write applications for embedded products.
    
    [Docs »](https://docs.platformio.org/en/latest/index.html)


## How it was made

In this section we will go over how this demo was made. It will not explain every step fully but will provide enough information that anyone should be able to recreate a version of this demo, even if they do not run it on the same type of servers or with the same embedded devices.

### Setting Up the Tools

First we will provide resources so that you can setup all the required servers. In this demo pipeline we require four servers. All these servers could be run on the same computer using some container software like Docker. 
Server setup in our demo video:
* Jenkins server ran a Raspeberry Pi running Raspbian
* Build server ran in a Docker container on a Ubuntu laptop
* Test server ran on a MacOS laptop
* Hardware test server / Jenkins ssh slave agent ran on a Raspeberry Pi running Raspbian

 #### Jenkins Server

There are many ways to setup a **Jenkins** server. I can run on virtually any device that can run the Java Runtime Environment (JRE). Docker is one of the easiest ways of starting a **Jenkins** server. 
* [Download Jenkins](https://www.jenkins.io/download/)
* [Installing Jenkins](https://www.jenkins.io/doc/book/installing/)

One caveat is that this server must also run **PlatformIO**. **PlatformIO** is python based and **PlatformIO** remote which is the only thing this server will use does not require the ability to compile and run a microcontroller compile suite, e.g. `avrdude`, `avr-gcc` etc. 
Note for example if you run **Jenkins** using their lts docker image it will run on alpine linux. At the current time **PlatformIO** cannot download the correct toolchain for atleast the Atmel/Microship chipsets. 

Next you will add the blueocean plugin. There is a compleate docker image availiable with Jenkins+Blueocean already setup. Just be aware of the caveat above.
* [Blue Ocean Setup](https://www.jenkins.io/doc/book/blueocean/getting-started/)
* [Creating a Pipeline](https://www.jenkins.io/doc/book/blueocean/creating-pipelines/)
When creating a pipeline you will directly connect it to your GitHub repo.

To be able to recive webhooks from GitHub you will have to add `http://ipAddress:port/github-webhook/` to your GitHub repo. Where `ipAddress:port` is the address where your **Jenkins** server is accessable from. A simple way to make a `localhost` port available to the internet is using a software like [ngrok](https://ngrok.com/)
* [Add GitHub Webhook to Jenkins Pipeline using ngrok](https://dzone.com/articles/adding-a-github-webhook-in-your-jenkins-pipeline)

Now to setup **PlatformIO**. The CLI supports most common OS. However it does not guarantee that a specific OS supports some specific toolchain. However on this server we are only using **PlatformIO Remote** or `pio remote`.
* [Installing PlatformIO](https://docs.platformio.org/en/latest/core/installation.html)

Caveat when using `pio remote` every agent and client must use the same version of Pyhton, i.e. `Python2` or `Python3`.

 #### Build Server

 The build server only needs to install **PlatformIO**. The same procedure as for the **Jenkins** server apply here. Only with the extra requirement that this server must be able to build the project using the required toolchain. For **AVR** projects **Windows**, **MacOS**, and **Ubuntu**/**Debian**-like OS will work (including **ARM** based once like **Rasbian**). Because this server will run a **PlatformIO Remote Agent** you must create an account and login to **PIO**.
 You can then run this script to generate a token which can be used by the **Jenkins Server** to send commands via `pio remote`.
 ```bash
pio account login
pio account token
```
* [`pio account` docs](https://docs.platformio.org/en/latest/plus/pio-account.html)

We also need to add this token as a Jenkins Credential. In our example this is a *secret text* called `BUILD_TOKEN`.
* [Using Credentials](https://www.jenkins.io/doc/book/using/using-credentials/)

You can then start the remote agent with:
```
pio remote agent start
```
* [`pio remote agent` docs](https://docs.platformio.org/en/latest/core/userguide/remote/cmd_agent.html)

For our demo video we used the following [Dockerfile](Dockerfile) and [docker-compose.yml](docker-compose.yml)

##### Platform Remote Overview 

![PlatformIO Remote Technology Architecture](images/pio-remote-architecture.png)

The image above is an overview of how **PlatformIO Remote** works. But the idea is that you have the two (green) entites in the middle, one *Agent* and one *Client*. Both are logged in to a **PlatformIO** account. A client (in our case the Jenkins Server) can then send commands via the **PIO Clound** to a *Remote Agent*. The remote framework makes sure that the relevant projects are in sync and can excute the commands on the *Remote Agent* (in this case a build command on the build server). The *Client* will then get the results of this command as if it ran the command locally. 

This is the system which enables us to use PIO in a scalable and automated manner. 

 #### Test Server

The setup here is exactly the same as for the Build Server. Only now we use a new account so we get a different token. We then setup a credential called `TEST_TOKEN`.

TODO: Add info about `platformio.ini` and device setup.

 #### Hardware Test Server

 This server needs to have **PlatformIO** installed as well as be accessable via SSH.
 We will setup this server as a **Jenkins** SSH slave agent.
 The **Jenkins** documentation has a howto set this up.
 * [Distributed Builds - Have master launch agent via SSH](https://wiki.jenkins.io/display/JENKINS/Distributed+builds#Distributedbuilds-Havemasterlaunchagentviassh)

 TODO: Add info on how we setup test rig.

### Pipeline Overview

![Pipeline Overview](images/pipeline-overview.png)
So this picture shows the overall pipeline we will be using.

1. First Some Action trigger a GitHub webhook
2. Jenkins Recives Webhook and starts the pipeline
    1. Build
         * Send `pio remote` command to Build Server
    2. Software Test 
         * Send `pio remote` command to Build Server
    2. Hardware Test 
         * Run as SSH slave agent
         * Run commands directly on server via SSH
         * Executes python test script.
    2. Deploy
         * If this was a merge / commit to the master branch
         * then deploy.
         * In our demo for simplicity we just used the slave agent to upload the new build to a specfic board. (Just as a proof of concept)

#### Build Step

```Jenkinsfile
stage('Build') {
    steps {
    sh '''pio account logout || true
PLATFORMIO_AUTH_TOKEN=${BUILD_TOKEN} pio remote run -r
'''
    }
}
```

TODO: Add Explanation

#### Software Test Step

```Jenkinsfile
stage('Software test') {
    steps {
    sh '''pio account logout || true
PLATFORMIO_AUTH_TOKEN=${TEST_TOKEN} pio remote test -e native -r'''
    sh '''pio account logout || true
PLATFORMIO_AUTH_TOKEN=${TEST_TOKEN} pio remote test -r'''
    }
}
```

TODO: Add Explanation

#### Hardware Test Step

```Jenkinsfile
stage('Hardware test') {
    agent {
    label 'PlatformIO-slave'
    }
    steps {
    sh '''/home/jenkins/.local/bin/pio run -e uno -t upload --upload-port /dev/ttyUSB0
sleep 5
python test_scripts/check.py'''
    }
}
```

TODO: Add Explanation

#### Deploy Step

```Jenkinsfile
stage('Deploy') {
    agent {
    label 'PlatformIO-slave'
    }
    when {
        branch "master"
    }
    steps {
    sh '''/home/jenkins/.local/bin/pio run -e megaatmega2560 -t upload --upload-port /dev/ttyUSB1
'''
    }
}
```

TODO: Add Explanation

## License

Distributed under the XXX License. See `LICENSE` for more information.


## Contact

Axel Boldt-Christmas - xmas1915@gmail.com
Carl Jensen - xxx@example.com

Project Link: [https://github.com/Callet91/DEMO_Jenkins_PlatformIO](https://github.com/Callet91/DEMO_Jenkins_PlatformIO)


## Acknowledgements

* []()
* []()
* []()
