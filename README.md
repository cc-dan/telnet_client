# Compilation
The following commands are meant to be executed on project root.

1. Compile libsocket: follow instructions on libsocket's GitHub page (https://github.com/cc-dan/libsocket). Said library's project root directory must be on the same level as this project.
2. Compile libtelnet:
```bash
cd lib/libtelnet && cmake -Bbuild -S. && cmake --build build/
```
3. Compile the main project:
```bash
cmake -Bbuild -S. && cmake --build build/
```
