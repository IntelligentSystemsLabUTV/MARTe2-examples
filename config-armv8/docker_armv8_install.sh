#!/bin/bash

# Docker Engine and Docker Compose installation script (updated as of today).
#
# Roberto Masocco <robmasocco@gmail.com>
# Alessandro Tenaglia <alessandro.tenaglia42@gmail.com>
#
# January 26, 2022

# Purge preexisting (i.e. not forward-compatible) Docker installations
echo "Purging old installations..."
sudo apt-get remove -y docker docker-engine docker.io containerd runc
if [[ -d /var/lib/docker ]]; then
  sudo rm -rf /var/lib/docker
fi
if [[ -d /var/lib/containerd ]]; then
  sudo rm -rf /var/lib/containerd
fi

# Install some dependencies for apt
echo "Installing dependencies..."
sudo apt-get update
sudo apt-get install -y \
  ca-certificates \
  curl \
  git \
  gnupg \
  lsb-release

# Download the installation script
echo "Downloading the installation script..."
curl -fsSL https://get.docker.com -o get-docker.sh

# REBOOT ??

# Install Docker
echo "Installing Docker..."
sudo sh get-docker.sh

# Docker version
docker version

# Install some dependencies for Docker compose
echo "Installing dependencies for Docker compose..."
sudo apt-get update
sudo apt-get install -y \
  libffi-dev \
  libssl-dev \
  python3 \
  python3-dev \
  python3-pip

# Install Docker Compose
sudo pip3 install docker-compose

# Enable Docker system srvice on boot up
sudo systemctl enable docker
