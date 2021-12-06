# DASH WRAPPER

## INSTALL

```bash
# install (Linux/ Unix only)
sudo apt install gcovr lcov

# install_docs
pip install jinja2 Pygments
sudo apt install doxygen

# setup
pip install conan
conan user

# prepare (top-level folder)
rm -rf build
mkdir build
cd build && conan install .. && cd ..
```

## CMAKE USAGE

```bash
cd build && cmake .. && cmake --build .
```

## CMAKE EXPLAINED

```cmake

```

## CONAN EXPLAINED

```txt
conan
```
