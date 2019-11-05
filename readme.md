# Building
## Debian 9
*NOTE*: The first command will not work with sudo.
`# echo 'deb http://download.opensuse.org/repositories/home:/strik/Debian_9.0/ /' > /etc/apt/sources.list.d/home:strik.list`
`# wget -nv https://download.opensuse.org/repositories/home:strik/Debian_9.0/Release.key -O /Release.key`
`# apt-key add - < Release.key`
`# apt-get update`
`# apt-get install make g++ pkg-config mesa-common-dev libgl-dev libpng-dev cc65`
