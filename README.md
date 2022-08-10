# Nike: fast LTLf synthesis

## Build from source

### CMake
We use CMake as a build tool. Please
check the [official website](https://cmake.org/)
to download it for your platform.

### Flex and Bison
The project uses Flex and Bison for parsing purposes.

Firse check that you have them: `whereis flex bison`

If no item occurs, then you have to install them:  
```sudo apt-get install -f flex bison```

### CUDD

The project depends on the CUDD library (version 3.0.0).

To install the CUDD library, run the following commands:

```shell script
wget https://github.com/whitemech/cudd/releases/download/v3.0.0/cudd_3.0.0_linux-amd64.tar.gz
tar -xf cudd_3.0.0_linux-amd64.tar.gz
cd cudd_3.0.0_linux-amd64
sudo cp -P lib/* /usr/local/lib/
sudo cp -Pr include/* /usr/local/include
```

Otherwise, build from source (customize `PREFIX` variable as you see fit).
```
git clone https://github.com/whitemech/cudd && cd cudd
PREFIX="/usr/local"
./configure --enable-silent-rules --enable-obj --enable-dddmp --prefix=$PREFIX
sudo make install
```

If you get an error about aclocal, this might be due to either
1. Not having automake:
   ```sudo apt-get install automake```
2. Needing to reconfigure, do this before configuring:
   ```autoreconf -i```
3. Using a version of aclocal other than 1.14:
   modify the version 1.14 in configure accordingly.
