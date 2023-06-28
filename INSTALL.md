## Install hul-common-lib
### Build example

```
mkdir amaneq-soft
cd  amaneq-soft
mkdir build install

git clone https://github.com/spadi-alliance/amaneq-soft.git amaneq-soft.src.git 

cmake \
   -S ./amaneq-soft.src.git \
   -B ./build \
   -DCMAKE_INSTALL_PREFIX=./install \
   -DCMAKE_PREFIX_PATH=<your hul_core_lib install dir>/install
cd build
make
make install
```

### Practical usage

We recommend to make a symbolic link the to hul-core-lib install directory inside the amaneq-soft install directory.
