# Shamblr

## Building

Handy script to execute ``cmake`` in vagrant:

```sh
# Linux target
vagrant ssh -c 'shamblr/run-cmake-linux'
# Windows target
vagrant ssh -c 'shamblr/run-cmake-windows'
```

Another handy script to execute ``make install`` in vagrant:

```sh
# Installs in bin/
vagrant ssh -c 'shamblr/run-make'
```
