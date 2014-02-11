#!/bin/bash

set -e

if [ ! -f ~/.vagrantonce ]
then
    apt-get update
    apt-get -qq --force-yes install make
    
    touch ~/.vagrantonce
fi
