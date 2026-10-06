# My_pgp (v0.1)

`my_pgp` est un outil en ligne de commande écrit en C++ qui chiffre et déchiffre un message lu sur l'entrée standard, avec six systèmes cryptographiques : **XOR**, **AES**, **RSA**, **PGP-XOR**, **PGP-AES** et **ElGamal**. Il permet aussi de générer des paires de clés RSA et ElGamal.

Projet réalisé dans le cadre de l'Epitech (projet MyPGP).

**Auteurs :** Ariel Amriou, Armand Lecomte, Natan Pereira & Pierrick Simon

## Sommaire

- [Fonctionnalités](#fonctionnalités)
- [Dépendances](#dépendances)
- [Compilation](#compilation)
- [Utilisation](#utilisation)
- [Exemples](#exemples)
- [Format des clés et des données](#format-des-clés-et-des-données)
- [Architecture](#architecture)
- [Tests](#tests)
- [Limites et points d'attention](#limites-et-points-dattention)
- [Licence](#licence)

## Fonctionnalités

| Système   | Type                  | Modes disponibles         |
|-----------|-----------------------|---------------------------|
| `xor`     | Symétrique (XOR)      |  `-c`, `-d`, `-b`         |
| `aes`     | Symétrique (AES)      |  `-c`, `-d`, `-b`         |
| `rsa`     | Asymétrique (RSA)     |  `-c`, `-d`, `-g P Q`     |
| `pgp-xor` | Hybride (RSA + XOR)   |  `-c`, `-d`, `-b`         |
| `pgp-aes` | Hybride (RSA + AES)   |  `-c`, `-d`, `-b`         |
| `elgamal` | Asymétrique (ElGamal) |  `-c`, `-d`, `-b`, `-g P` |

Le principe du mode hybride (`pgp-*`) : une clé symétrique aléatoire chiffre le message, puis cette clé est elle-même chiffrée avec RSA. Seul le détenteur de la clé privée RSA peut retrouver la clé symétrique.

## Dépendances

### Compilation

| Dépendance | Version / détail | Rôle |
|------------|------------------|------|
| **clang++** | Supportant `-std=c++26` (testé avec clang 22.1.2) | Compilateur imposé par le `Makefile` (`CXX = clang++`) |
| **Boost** | Testé avec `libboost-dev` 1.83 | En-têtes uniquement : `multiprecision/cpp_int.hpp` (entiers de taille arbitraire), `random` (générateur Mersenne Twister), `integer/mod_inverse.hpp` (inverse modulaire) |
| **make** | GNU make | Construction du projet |

Installation sous Debian / Ubuntu :

```sh
sudo apt-get install clang make libboost-dev
```

### Tests

| Dépendance | Rôle |
|------------|------|
| **Criterion** (`libcriterion-dev`) | Framework de tests unitaires et fonctionnels. Requis uniquement pour `make tests_run` |
| **gcov / --coverage** | Utilisé par la cible `tests_run` pour la couverture de code |

```sh
sudo apt-get install libcriterion-dev
```

### Intégration continue

Le workflow `.github/workflows/mirror.yml` compile le projet, vérifie que `my_pgp` est exécutable, lance `make tests_run` dans l'image `epitechcontent/epitest-docker`, puis pousse le dépôt vers le miroir Epitech.

## Compilation

```sh
make              # compile my_pgp
make tests_run    # compile et lance la suite de tests (Criterion requis)
make clean        # supprime les fichiers objets (.o) et les binaires de test
make fclean       # clean + supprime my_pgp
make re           # fclean puis all
```

Le binaire produit est `./my_pgp`.

## Utilisation

```
./my_pgp CRYPTO_SYSTEM MODE [OPTIONS] [key]
```

Le message est lu sur l'entrée standard (`stdin`). Le résultat est écrit sur la sortie standard.

### Systèmes (`CRYPTO_SYSTEM`)

`xor`, `aes`, `rsa`, `pgp-xor`, `pgp-aes`, `elgamal`

### Modes

| Option | Effet |
|--------|-------|
| `-c` | Chiffre le message (qui est en clair) |
| `-d` | Déchiffre le message (qui est chiffré) |
| `-g P Q` | RSA uniquement : ne lit pas de message, génère une paire de clés RSA à partir des nombres premiers P et Q |
| `-g P[:graine]` | ElGamal uniquement : ne lit pas de message, génère un générateur et une paire de clés à partir du nombre premier P, avec une graine optionnelle |

Exactement un mode doit être présent (`-c`, `-d` ou `-g`).

### Options

| Option | Effet |
|--------|-------|
| `-b` | Chiffre un seul bloc (XOR, AES, PGP, ElGamal). Le message et la clé symétrique doivent alors avoir la même taille |
| `-h` | Affiche l'aide et quitte |

### Clé (`key`)

Argument positionnel obligatoire pour `-c` et `-d`, incompatible avec `-g`. Son format dépend du système (voir [Format des clés et des données](#format-des-clés-et-des-données)).

### Codes de sortie

| Code | Signification |
|------|---------------|
| `0` | Succès (ou aide affichée) |
| `84` | Erreur (arguments invalides, clé mal formée, message non déchiffrable, etc.). Le message d'erreur est écrit sur la sortie d'erreur |

## Exemples

Les sorties ci-dessous ont été obtenues avec le binaire compilé dans ce dépôt.

### XOR

```sh
$ echo -n "Hello PGP" | ./my_pgp xor -c 0a0b0c0d
0000005d4d5b2c6266676945
$ echo -n "0000005d4d5b2c6266676945" | ./my_pgp xor -d 0a0b0c0d
Hello PGP
```

### AES

```sh
$ echo -n "Hello PGP" | ./my_pgp aes -c 000102030405060708090a0b0c0d0e0f
7e518fec8e34aff92edf4e6b4b303b49
$ echo -n "7e518fec8e34aff92edf4e6b4b303b49" | ./my_pgp aes -d 000102030405060708090a0b0c0d0e0f
Hello PGP
```

### RSA

Génération de clés à partir de deux nombres premiers (valeurs en hexadécimal little-endian, voir plus bas) :

```sh
$ ./my_pgp rsa -g 1f621ef7a931d0b235 739781e77a2c0f6631
public key: 010001-ed5c2559bc979c9c99fb857d084447a25c0a
private key: bdc41bceafe4b2f716c22123790f1f5b9801-ed5c2559bc979c9c99fb857d084447a25c0a
```

Chiffrement avec la clé publique, déchiffrement avec la clé privée :

```sh
$ echo -n "AB" | ./my_pgp rsa -c 010001-ed5c2559bc979c9c99fb857d084447a25c0a
4385e6dd9a3c5836941ba096f09f87100f01
$ echo -n "4385e6dd9a3c5836941ba096f09f87100f01" | ./my_pgp rsa -d bdc41bceafe4b2f716c22123790f1f5b9801-ed5c2559bc979c9c99fb857d084447a25c0a
AB
```

### PGP-AES

Le chiffrement produit **deux lignes** : la clé AES chiffrée avec RSA, puis le message chiffré avec AES.

```sh
$ echo -n "Hello PGP" | ./my_pgp pgp-aes -c 000102030405060708090a0b0c0d0e0f:010001-ed5c2559bc979c9c99fb857d084447a25c0a
9fbcd13f5daededb21843ec6c4bb92814201
7e518fec8e34aff92edf4e6b4b303b49
```

Le déchiffrement prend sur `stdin` **uniquement la seconde ligne**, et la clé est `clé_AES_chiffrée:clé_privée_RSA` :

```sh
$ echo -n "7e518fec8e34aff92edf4e6b4b303b49" | ./my_pgp pgp-aes -d "9fbcd13f5daededb21843ec6c4bb92814201:bdc41bceafe4b2f716c22123790f1f5b9801-ed5c2559bc979c9c99fb857d084447a25c0a"
Hello PGP
```

### PGP-XOR

Même principe que PGP-AES, avec une clé XOR à la place de la clé AES :

```sh
$ echo -n "Hello PGP" | ./my_pgp pgp-xor -c 0a0b0c0d:010001-ed5c2559bc979c9c99fb857d084447a25c0a
1a021a734d84b29f9b34f8481e2813700804
0000005d4d5b2c6266676945
$ echo -n "0000005d4d5b2c6266676945" | ./my_pgp pgp-xor -d "1a021a734d84b29f9b34f8481e2813700804:bdc41bceafe4b2f716c22123790f1f5b9801-ed5c2559bc979c9c99fb857d084447a25c0a"
Hello PGP
```

### ElGamal

Génération de clés à partir du nombre premier `0101` (soit 257). La graine `:02` rend le résultat reproductible :

```sh
$ ./my_pgp elgamal -g 0101:02
Generator: 03
Private key: 70
Public key: e1
```

Le format de clé est `p-g-clé` :

```sh
$ echo -n "Hi" | ./my_pgp elgamal -c 0101-03-e1:02
a1-0900ee00
$ echo -n "a1-0900ee00" | ./my_pgp elgamal -d 0101-03-70
Hi
```

Sans graine (`-g 0101` ou `-c 0101-03-e1`), le tirage est aléatoire : chaque appel donne un résultat différent, mais le déchiffrement reste correct.

## Format des clés et des données

### Nombres : hexadécimal little-endian

**Tous les nombres** passés en argument (valeurs de `-g`, composantes des clés RSA et ElGamal) sont lus et écrits en **hexadécimal, octets dans l'ordre little-endian**. Exemple : `0101` vaut 257 et `3d` vaut 61. Pour `-g`, il faut donc donner les nombres premiers en hexadécimal little-endian, et non en décimal.

Exemple : pour générer une paire RSA à partir de p = 61 et q = 53 (décimal) :

```sh
$ ./my_pgp rsa -g 3d 35
public key: 0101-a10c
private key: ad-a10c
```

> Note : l'aide intégrée (`public/help.txt`) indique « prime number P and Q » sans préciser la base. Le comportement réel est celui décrit ici.

## Architecture

```
.
├── Makefile                  # Build, tests, nettoyage
├── public/help.txt           # Texte affiché par -h
├── src/
│   ├── Main.cpp              # Point d'entrée, gestion des codes de sortie
│   ├── ArgsParser.cpp        # Extraction des flags et valeurs de la ligne de commande
│   ├── MyPgp.cpp             # Orchestration : parsing, dispatch, conversions hex / little-endian
│   └── Cript/
│       ├── Xor/Xor.cpp
│       ├── AES/AES.cpp
│       ├── RSA/RSA.cpp
│       ├── ElGamal/ElGamal.cpp
│       └── (PGP est un template header-only)
├── include/
│   ├── ACipher.hpp           # Type de fonction de chiffrement commun (CipherFn)
│   ├── ArgsParser.hpp        # Classe ArgsParser (templates)
│   ├── Exception.hpp         # MyPgpException et dérivées
│   ├── Matrix.hpp            # Matrice 4x4 utilisée par AES
│   ├── MyPgp.hpp             # Classe MyPgp, enums CryptoSystem / Mode, table de dispatch
│   └── Cript/
│       ├── PGP/PGP.hpp       # Schéma hybride, template sur le chiffrement symétrique
│       ├── Xor/Xor.hpp
│       ├── AES/AES.hpp
│       ├── RSA/RSA.hpp
│       └── ElGamal/ElGamal.hpp
└── tests/                    # Tests Criterion (unitaires et fonctionnels)
```

## Tests

```sh
make tests_run
```

La cible `tests_run` supprime d'abord les anciens binaires et fichiers de couverture, compile le projet avec `--coverage` et `-lcriterion`, puis exécute la suite.

| Fichier | Nombre de tests | Contenu |
|---------|-----------------|---------|
| `tests/tests_init.cpp` | 1 | Test d'exemple de l'infrastructure Criterion |
| `tests/tests_xor.cpp` | 15 | Tests unitaires XOR (vecteurs du sujet, aller-retour, bloc) |
| `tests/tests_aes.cpp` | 4 | Tests unitaires AES (un et deux blocs, tailles de clé invalides) |
| `tests/tests_rsa.cpp` | 21 | Lambda de Carmichael, génération de clés RSA, chiffrement |
| `tests/tests_elgamal.cpp` | 24 | Recherche de générateur, plage de la clé privée, graine reproductible, chiffrement |
| `tests/tests_pgp.cpp` | 32 | Schéma hybride PGP-AES et PGP-XOR |
| `tests/tests_functionnal.cpp` | 10 | Tests fonctionnels de bout en bout |
| `tests/tests_error_handling.cpp` | 15 | Parsing des arguments et cas d'erreur (`-h`, système inconnu, etc.) |

Le dossier `tests/files/` contient des messages et des données chiffrées de référence utilisés par les tests.

## Limites et points d'attention

> **Projet pédagogique : ne pas utiliser pour protéger de vraies données.**

- **XOR** : chiffrement trivialement cassable dès que la clé est réutilisée ou connue partiellement.
- **AES** : aucun mode d'opération ni vecteur d'initialisation (IV). Chaque bloc de 16 octets est chiffré indépendamment, ce qui revient à un mode de type ECB : des blocs identiques donnent des chiffrés identiques.
- **RSA** : « textbook RSA », sans padding (OAEP, PKCS#1…). Le chiffrement est déterministe, et le message entier doit être strictement inférieur à `n`. Avec `n` de 140 bits environ, cela limite les messages à environ 17 octets.
- **ElGamal** : chaque caractère doit être strictement inférieur au nombre premier `p`. Il faut donc un `p` supérieur à 255 pour chiffrer du texte. Une graine fournie rend la clé de session prévisible.
- **Génération de clés RSA** : les nombres P et Q ne sont pas vérifiés comme premiers.
- **Padding par `\0`** : XOR et AES retirent les `\0` finaux au déchiffrement. Un message se terminant par des octets nuls perd ces octets.
- **Base des arguments** : tous les nombres sont en hexadécimal little-endian, y compris `-g`, alors que l'aide mentionne simplement P et Q.
- **Portabilité** : `isatty` et `unistd.h` imposent un système POSIX.
- **Makefile** : la cible `debug` duplique `$(OBJ)` et utilise `CFLAGS`, qui n'est pas utilisé par la règle de compilation. La variable `SERVER_NAME` de `fclean` n'est pas définie.

## Licence

Ce projet est distribué sous licence **Creative Commons Attribution-ShareAlike 4.0 International** (CC BY-SA 4.0).
