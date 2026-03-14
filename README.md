## Getting Started Guide

This guide walks you through setting up the Docker environment and
performing a basic "kick-the-tires" test to verify the artifact
functions correctly. This initial phase is designed to take less than
30 minutes to complete.

### 1. Prerequisites
* **Docker:** Ensure Docker is installed and running on your host
  machine.
* **OS Compatibility:** Compatible with Linux.

### 2. Environment Setup
Navigate to the directory (Dualis) of the artifact (where the `Dockerfile`
is located) and build the image:

```bash
docker image build -t dualis:latest .
```
and run the image :

```bash
docker run --rm -it dualis:latest
```

### 3. Running Basic Tests



## Step-by-Step instructions for evaluations
