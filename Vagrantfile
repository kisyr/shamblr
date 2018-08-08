$provision = <<-SCRIPT
apt-get update && apt-get -y upgrade && apt-get -y install gcc g++ gcc-mingw-w64 g+-mingw-w64 curl cmake git xorg-dev libgl1-mesa-dev libogg-dev libvorbis-dev
SCRIPT

Vagrant.configure("2") do |config|
	config.vm.provider "virtualbox"
	config.vm.box = "ubuntu/xenial64"
	config.vm.provision "shell", inline: $provision
	config.vm.synced_folder "./", "/home/vagrant/shamblr"
end
