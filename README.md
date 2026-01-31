# OOP_Space_Invaders
Repozitorij za projekt iz OOP

# Bilješka za Assets direktorij
Igra koristi `Assets/` direktorij s relativnim putanjama, npr. `Assets/space_1.png`.
Dakle, `Assets/` mora biti u istom direktoriju kao i izvršna (.exe) datoteka.

# Build i pokretanje (Linux)

### Dependencies (Testirano na Ubuntu/Kubuntu sustavima)
Potrebno je pokrenuti sljedeće naredbe:

```bash
sudo apt update
sudo apt install -y build-essential cmake git \
  libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev \
  libgl1-mesa-dev libasound2-dev
```
## Build
Iz root putanje (gdje se nalazi CMakeLists.txt):
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```
## Pokretanje
```bash
./build/Space-Invaders
```
